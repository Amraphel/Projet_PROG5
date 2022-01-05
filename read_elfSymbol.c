#include "read_elfSymbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

unsigned char * renvoyer_nom_du_symbole(int indice, FILE * file)
{
    Elf32Hdr header;
    fread(&header, 1, sizeof(header), file);
    int shnum =reverse_endianess(header.e_shnum)>>16;
    int shstrndx  =reverse_endianess(header.e_shstrndx)>>16;
    int i=0;
    Elf32_Shdr* tab_sec =read_elf_section(file,header);
    
    Elf32_Shdr symtab;
    Elf32_Shdr strtab;
    Elf32_Sym symtable;

    for(int i=0;i<shnum;i++){   
        if(reverse_endianess(tab_sec[i].sh_type)==SHT_SYMTAB) {
            symtab=tab_sec[i];
        }
        if(reverse_endianess(tab_sec[i].sh_type)==SHT_STRTAB && i!=shstrndx){
            strtab=tab_sec[i];
        }
    }
    
    fseek(file,reverse_endianess(strtab.sh_offset), SEEK_SET);
    unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*reverse_endianess(strtab.sh_size));
    
    fread(strtable, sizeof(char), strtab.sh_size, file);
    fseek(file,reverse_endianess(symtab.sh_offset), SEEK_SET);
    i=0;
    while(i<=indice)
    {
        fread(&symtable, sizeof(Elf32_Sym), 1, file);
        i++;
    }
    //printf(" %s", strtable+(reverse_endianess(symtable.st_name)));
    return strtable+(reverse_endianess(symtable.st_name));
}

void affiche_table_Symboles(FILE *file){

    Elf32Hdr header;
    fread(&header, 1, sizeof(header), file);
    int shnum =reverse_endianess(header.e_shnum)>>16;
    int shstrndx  =reverse_endianess(header.e_shstrndx)>>16;
    int i=0;
    int taille;
    Elf32_Shdr* tab_sec =read_elf_section(file,header);
    Elf32_Shdr symtab;
    Elf32_Shdr strtab;
    Elf32_Sym symtable; // 1    symbole
    char* symbole_type="";
    char* symbole_bind="";
    char* symbole_vis="";

    for(i=0;i<shnum;i++){   
        if(reverse_endianess(tab_sec[i].sh_type)==SHT_SYMTAB) {
            symtab=tab_sec[i];
        }
        if(reverse_endianess(tab_sec[i].sh_type)==SHT_STRTAB && i!=shstrndx){
            strtab=tab_sec[i];
        }
    }
    fseek(file,reverse_endianess(strtab.sh_offset), SEEK_SET); // on se place au debut de la table des strings
    unsigned char* strtable = (unsigned char *)malloc(sizeof(unsigned char)*reverse_endianess(strtab.sh_size));
    
    fread(strtable, sizeof(char), strtab.sh_size, file);
    fseek(file,reverse_endianess(symtab.sh_offset), SEEK_SET); // on se place au debut de la table des symboles
    taille=(reverse_endianess(symtab.sh_size)/reverse_endianess(symtab.sh_entsize)); // nombre de symboles
    
    // l'affichage
    printf("\nLa table de symboles « .symtab » contient %d entrées :\n",taille);
    printf("%7s\t%9s %s %s\t%2s\t%s\t%4s %s","Num:","Valeur","Tail","Type","Lien","Vis","Ndx","Nom");
    printf("\n");
    i=0;
    while(i<taille){
        fread(&symtable, sizeof(Elf32_Sym), 1, file); //recuperer le symbole à chaque fois
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
        switch(reverse_endianess(symtable.st_other)>>4){
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
        printf(" %08x",reverse_endianess(symtable.st_value));
        printf("%6d",reverse_endianess(symtable.st_size)>>16);
        printf(" %s",symbole_type);
        printf("\t%s",symbole_bind);
        printf("\t%6s",symbole_vis);
        
        int indexe = reverse_endianess(symtable.st_shndx)>>16;

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

        printf(" %s", strtable+(reverse_endianess(symtable.st_name)));       
        printf("\n");   
        i++;
    }
    free(tab_sec);
    free(strtable);
}

int main(int argc, char *argv[])
{
    char *elfile = argv[1];
    FILE *file = fopen(elfile, "rb");
    if(!file){
         printf("erreur de lecture");
    } else {
            affiche_table_Symboles(file);
           // unsigned char *valeur =renvoyer_nom_du_symbole(20,file);
           // printf("%s",valeur);
    }
    fclose(file);
    return 0;
}