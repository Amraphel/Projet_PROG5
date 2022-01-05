#include "read_elfSection.h"

Elf32_Shdr * read_elf_section(FILE *elfFile, Elf32Hdr header)
{
    Elf32_Shdr * sections = NULL;
    sections = malloc(sizeof(Elf32_Shdr) * reverse_endianess(header.e_shnum, header, 1));

    if(sections){
    
        fseek(elfFile, reverse_endianess(header.e_shoff, header, 0), SEEK_SET);

        for (size_t i = 0; i < reverse_endianess(header.e_shnum, header, 1); i++) {
            fread(&sections[i], 1, sizeof(Elf32_Shdr), elfFile);
        }
    }else{
        fprintf(stderr,"Erreur malloc sections");
        exit(1);
    }


    return sections;
}

void print_elf_section(Elf32Hdr header, Elf32_Shdr * sections, FILE* elfFile){

    printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%02x: \n\n", reverse_endianess(header.e_shnum, header, 1), reverse_endianess(header.e_shoff, header, 0));
    printf("En-têtes de section :\n");
    printf("%2s\t%-15s\t%-14s\t%-8s\t%6s\t%6s\t%2s\t%3s\t%2s\t%3s\t%2s","[Nr]","Nom","Type","Adr","Décala.","Taille","ES","Fan","LN","Inf","Al");
    printf("\n");

    char* name = "";
    int * flag_ind = malloc(sizeof(int));
    for (size_t i = 0; i < reverse_endianess(header.e_shnum, header, 1); i++)
    {
        //number
        printf("[%2ld]\t",i);

        //section name
        name = getSectionName(header,elfFile,i);
        printf("%-15s\t",name);
        
        // Type
        ELF32_Word type = reverse_endianess(sections[i].sh_type, header, 0);
        char * typeAffich = get_section_type(type, name);
        printf("%-14s\t",typeAffich);
        
        printf("%08x\t",reverse_endianess(sections[i].sh_addr, header, 0));
        printf("%06x\t",reverse_endianess(sections[i].sh_offset, header, 0));
        printf("%06x\t",reverse_endianess(sections[i].sh_size, header, 0));
        printf("%02x\t",reverse_endianess(sections[i].sh_entsize, header, 0));

        ELF32_Word sh_flags = reverse_endianess(sections[i].sh_flags, header, 0);
        char * flag = get_section_flag(sh_flags,flag_ind);

        while (*flag_ind >= 0)
        {
            printf("%c",flag[*flag_ind]);
            (*flag_ind)--;
        }
        free(flag);
        printf("\t");
      
        printf("%2d\t",reverse_endianess(sections[i].sh_link, header, 0));
        printf("%3d\t",reverse_endianess(sections[i].sh_info, header, 0));
        printf("%2d\t",reverse_endianess(sections[i].sh_addralign, header, 0));

        
        printf("\n");
    }

    free(flag_ind);
    
    printf("Clé des fanions :\nW (écriture), A (allocation), X (exécution),M (fusion), S (chaînes), I (info),\nL (ordre des liens), O (traitement supplémentaire par l'OS requis),G (groupe),\nT (TLS), x (inconnu), o (spécifique à l'OS), E (exclu),\np (processor specific)");
    printf("\n");
}

char * getSectionName(Elf32Hdr header, FILE* elfFile, int i){

    Elf32_Shdr sectionStrTab;

    fseek(elfFile, reverse_endianess(header.e_shoff, header, 0) + reverse_endianess(header.e_shstrndx, header, 1) * reverse_endianess(header.e_shentsize, header, 1), SEEK_SET);
    fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);
    char* SectNames = NULL;
    SectNames = malloc(reverse_endianess(sectionStrTab.sh_size, header, 0));
    fseek(elfFile, reverse_endianess(sectionStrTab.sh_offset, header, 0), SEEK_SET);
    fread(SectNames, 1, reverse_endianess(sectionStrTab.sh_size, header, 0), elfFile);

    char* name = "";
    //section name
    fseek(elfFile, reverse_endianess(header.e_shoff, header, 0) + i * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);

        // print section name
    if (reverse_endianess(sectionStrTab.sh_name, header, 0)){
        name = SectNames + reverse_endianess(sectionStrTab.sh_name, header, 0);
    }
    
    return name;
}


char * get_section_type(ELF32_Word sh_type, char * name){

    char * type = NULL;

    switch (sh_type)
    {
    case 0:
        type = "NULL";
        break;
    case 1:
        type = "PROGBITS";
        break;
    case 2:
        type = "SYMTAB";
        break;
    case 3:
        type = "STRTAB";
        break;
    case 4:
        type = "RELA";
        break;
    case 5:
        type = "HASH";
        break;
    case 6:
        type = "DYNAMIC";
        break;
    case 7:
        type = "NOTE";
        break;
    case 8:
        type = "NOBITS";
        break;
    case 9:
        type = "REL";
        break;
    case 10:
        type = "SHLIB";
        break;
    case 11:
        type = "DYNSYM";
        break;
    case 14:
        type = "INIT_ARRAY";
        break;
    case 15:
        type = "FINI_ARRAY";
        break;
    case 16:
        type = "PREINIT_ARRAY";
        break;
    case 17:
        type = "GROUP";
        break;
    case 18:
        type = "SYMTAB_SHNDX";
        break;
    case 0x70000000:
        type = "LOPROC";
        break;
    case 0x7fffffff:
        type = "HIPROC";
        break;
    case 0x80000000:
        type = "LOUSER";
        break;
    case 0xffffffff:
        type = "HIUSER";
        break;
    default:
        if(strcmp(name,".ARM.attributes") == 0){
            type = "ARM_ATTRIBUTES";
        }else{
            type = "";
        }
        break;
    }
    
    return type;
}

char * get_section_flag(ELF32_Word sh_flags, int * flag_ind){
    char * flag = malloc(sizeof(char)*3);
    int n = 15;
    *flag_ind = -1;
    int j = n-1;
    long int tmp;
    long int listFlag[15] = { 0x1, 0x2, 0x4, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x0ff00000, 0x10000000, 0x40000000, 0x80000000, 0xf0000000};
    long int flagValue;
    flagValue = sh_flags;
    if(flagValue){
        while(flagValue > 0 && j >= 0){
            if(listFlag[j] <= flagValue){
                (*flag_ind)++;
                flagValue = flagValue - listFlag[j];
                tmp = listFlag[j];
                switch (tmp)
                {
                    case 0x1:
                        flag[*flag_ind] = 'W';
                        break;
                    case 0x2:
                        flag[*flag_ind] = 'A';
                        break;
                    case 0x4:
                        flag[*flag_ind] = 'X';
                        break;
                    case 0x10:
                        flag[*flag_ind] = 'M';
                        break;
                    case 0x20:
                        flag[*flag_ind] = 'S';
                        break;
                    case 0x40:
                        flag[*flag_ind] = 'I';
                        break;
                    case 0x80:
                        flag[*flag_ind] = 'L';
                        break;
                    case 0x100:
                        flag[*flag_ind] = 'O';
                        break;
                    case 0x200:
                        flag[*flag_ind] = 'G';
                        break;
                    case 0x400:
                        flag[*flag_ind] = 'T';
                        break;
                    case 0x0ff00000:
                        flag[*flag_ind] = 'o';
                        break;
                    case 0x10000000:
                        flag[*flag_ind] = 'x'; //manquant
                        break;
                    case 0x40000000:
                        flag[*flag_ind] = 'x'; //manquant
                        break;
                    case 0x80000000:
                        flag[*flag_ind] = 'E';
                        break;
                    case 0xf0000000:
                        flag[*flag_ind] = 'p';
                        break;
                    default:
                        flag[*flag_ind] = 'x';
                        break;
                }
                
            }
            j--;
        }
    }else{
        (*flag_ind)++;
        flag[*flag_ind] = ' ';      
    }

    return flag;
}

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "rb");
    if(!file){
        printf("erreur de lecture");
    } else {
        Elf32Hdr header = read_elf_header(file);
        Elf32_Shdr * sections = NULL;
        sections = read_elf_section(file, header);
        print_elf_section(header,sections,file);
        free(sections);
    }

    fclose(file);
    return 0;
}