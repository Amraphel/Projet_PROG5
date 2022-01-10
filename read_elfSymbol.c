#include "read_elfSymbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//---------------------------------------------------------------------------
int get_taille_table_symbole (FILE * file, Elf32_Shdr* tab_sec, Elf32Hdr header){
    int shnum =header.e_shnum;
    int i=0;
    Elf32_Shdr symtab=tab_sec[0];
    for(i=0;i<shnum;i++){   
        if(tab_sec[i].sh_type==SHT_SYMTAB) {
            symtab=tab_sec[i];
        }

    }

    i=0;
    int taille=(symtab.sh_size/symtab.sh_entsize);
    return taille;
}

Elf32_Sym lire_un_symbole(FILE * file, Elf32Hdr header){
    Elf32_Sym symbole;

    ELF32_Word st_name;
    fread(&st_name,1, sizeof(ELF32_Word), file);

    ELF32_Addr st_value;
    fread(&st_value,1, sizeof(ELF32_Addr), file);

    ELF32_Word st_size;
    fread(&st_size,1, sizeof(ELF32_Word), file);

    unsigned char st_info;
    fread(&st_info,1, sizeof(unsigned char), file);

    unsigned char st_other;
    fread(&st_other,1, sizeof(unsigned char), file);

    ELF32_Half st_shndx;
    fread(&st_shndx,1, sizeof(ELF32_Half), file);

    symbole.st_name=reverse_endianess(st_name,header,0);
    symbole.st_value=reverse_endianess(st_value,header,0);
    symbole.st_size=st_size;
    symbole.st_info=st_info;
    symbole.st_other=reverse_endianess(st_other,header,0);
    symbole.st_shndx=reverse_endianess(st_shndx,header,1);
    return symbole;
}

Elf32_Sym*  renvoyer_table_sym(FILE * file, Elf32Hdr header , Elf32_Shdr* tab_sec)
{
    int shnum =header.e_shnum;
    int i=0;
    Elf32_Shdr symtab=tab_sec[0];
    for(i=0;i<shnum;i++){   
        if(tab_sec[i].sh_type==SHT_SYMTAB) {
            symtab=tab_sec[i];
        }

    }
    fseek(file,symtab.sh_offset, SEEK_SET);
    i=0;
    int taille=(symtab.sh_size/symtab.sh_entsize);

    Elf32_Sym* symtable = malloc(sizeof(Elf32_Sym)*taille);
    Elf32_Sym sym;

    while(i<taille){
        sym= lire_un_symbole(file, header);
        symtable[i]=sym;
        i++;
    }
    return symtable;
}
unsigned char * renvoyer_nom_du_symbole(int indice, FILE * file,Elf32Hdr header,Elf32_Shdr* tab_sec)
{   
    int shnum =header.e_shnum;
    int shstrndx  =header.e_shstrndx;
    int i=0;
    Elf32_Shdr symtab=tab_sec[0];
    Elf32_Shdr strtab=tab_sec[0];
    Elf32_Sym symtable;

    for(int i=0;i<shnum;i++){   
        if(tab_sec[i].sh_type==SHT_SYMTAB) {
            symtab=tab_sec[i];
        }
        if(tab_sec[i].sh_type==SHT_STRTAB && i!=shstrndx){
            strtab=tab_sec[i];
        }
    }
    
    fseek(file,strtab.sh_offset, SEEK_SET);
    unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*strtab.sh_size);
    
    fread(strtable, sizeof(char), strtab.sh_size, file);
    fseek(file,symtab.sh_offset, SEEK_SET);
    i=0;
    while(i<=indice)
    {
        symtable=lire_un_symbole(file, header);
        i++;
    }
    strtable=strtable+symtable.st_name;
    return strtable;

    //free(tab_sec);
}
//---------------------------------------------------------------------------
void affiche_table_Symboles(FILE *file,Elf32_Shdr* tab_sec,Elf32Hdr header, Elf32_Sym * tab_sym){
    int i=0;
    int taille;
    
    char* symbole_type="";
    char* symbole_bind="";
    char* symbole_vis="";


    taille= get_taille_table_symbole(file, tab_sec, header); // nombre de symboles
    
    // l'affichage
    printf("La table de symboles « .symtab » contient %d entrées :\n",taille);
    printf("%7s\t%9s %s %s\t%2s\t%s\t%4s %s","Num:","Valeur","Tail","Type","Lien","Vis","Ndx","Nom");
    printf("\n");
    i=0;
    while(i<taille){
        Elf32_Sym symtable= tab_sym[i];
        switch(ELF32_ST_TYPE(symtable.st_info)){
            case STT_NOTYPE : symbole_type="NOTYPE";
                break;
            case STT_OBJECT : symbole_type="OBJECT";
                break;
            case STT_FUNC : symbole_type="FUNC";
                break;
            case STT_SECTION : symbole_type="SECTION";
                break;
            case STT_FILE : symbole_type="FILE";
                break;
        }
        switch(ELF32_ST_BIND(symtable.st_info)){
            case STB_LOCAL : symbole_bind= "LOCAL";
                break;
            case STB_GLOBAL : symbole_bind="GLOBAL";
                break;
            case STB_WEAK : symbole_bind=  "WEAK";
                break;
        }
        switch(symtable.st_other>>4){
            case STV_DEFAULT : symbole_vis="DEFAULT";
                break;
            case STV_INTERNAL : symbole_vis="INTERNAL";
                break;
            case STV_HIDDEN : symbole_vis="HIDDEN";
                break;
            case STV_PROTECTED : symbole_vis="PROTECTED";
                break;
            default : symbole_vis="ERREUR";
                break;
        }
        printf("%6d:",i);
        printf(" %08x",symtable.st_value);
        printf("%6d",symtable.st_size);
        printf(" %s",symbole_type);
        printf("\t%s",symbole_bind);
        printf("\t%6s",symbole_vis);
        int indexe = symtable.st_shndx;
        if(indexe>0){
            printf("%5d",indexe);
        }
        else{
            if(indexe==SHN_UNDEF)
            {
                printf("%5s","UND");
            }
            else {
                printf("%5s","ABS");
            }
        }

        printf(" %s",renvoyer_nom_du_symbole(i,file, header,tab_sec));       
        printf("\n");   
        i++;
    }
    printf("\n");   

}

//---------------------------------------------------------------------------
// int main(int argc, char *argv[])
// {
//     char *elfile = argv[1];
//     FILE *file = fopen(elfile, "rb");
//     if(!file){
//         printf("erreur de lecture");
//     } else {
//         Elf32Hdr header;
//         fread(&header, 1, sizeof(header), file);
//         Elf32_Shdr* tab_sec =read_elf_section(file,header);
//         affiche_table_Symboles(file,tab_sec,header);
//         // unsigned char *valeur =renvoyer_nom_du_symbole(20,file,header,tab_sec);
//         // printf("%s",valeur);
//         free(tab_sec);
//     }
//     fclose(file);
//     return 0;
// }