#include "read_elfSection.h"



Elf32_Shdr lire_une_section(FILE* file, Elf32Hdr header){
    Elf32_Shdr sections;

    ELF32_Word sh_name;
    fread(&sh_name,1, sizeof(ELF32_Word), file);
    ELF32_Word sh_type;
    fread(&sh_type,1, sizeof(ELF32_Word), file);
    ELF32_Word sh_flags;
    fread(&sh_flags,1, sizeof(ELF32_Word), file);
    ELF32_Addr sh_addr;
    fread(&sh_addr,1, sizeof(ELF32_Word), file);
    ELF32_Off sh_offset;
    fread(&sh_offset,1, sizeof(ELF32_Word), file);
    ELF32_Word sh_size;
    fread(&sh_size,1, sizeof(ELF32_Word), file);
    ELF32_Word sh_link;
    fread(&sh_link,1, sizeof(ELF32_Word), file);
    ELF32_Word sh_info;
    fread(&sh_info,1, sizeof(ELF32_Word), file);
    ELF32_Word sh_addralign;
    fread(&sh_addralign,1, sizeof(ELF32_Word), file);
    ELF32_Word sh_entsize;
    fread(&sh_entsize,1, sizeof(ELF32_Word), file);

    //On applique les modifications nécéssaires auc futures traitements
    sections.sh_name=reverse_endianess(sh_name,header,0);
    sections.sh_type=reverse_endianess(sh_type,header,0);
    sections.sh_flags=reverse_endianess(sh_flags,header,0);
    sections.sh_addr=reverse_endianess(sh_addr,header,0);
    sections.sh_offset=reverse_endianess(sh_offset,header,0);
    sections.sh_size=reverse_endianess(sh_size,header,0);
    sections.sh_link=reverse_endianess(sh_link,header,0);
    sections.sh_info=reverse_endianess(sh_info,header,0);
    sections.sh_addralign=reverse_endianess(sh_addralign,header,0);
    sections.sh_entsize=reverse_endianess(sh_entsize,header,0);
    return sections;
}

Tab_Sec * read_elf_section(FILE *file, Elf32Hdr header)
{
    Tab_Sec * sections = NULL;
    sections = malloc(sizeof(Tab_Sec) * header.e_shnum);
    if(sections){
    
        fseek(file, header.e_shoff, SEEK_SET);

        for (size_t i = 0; i < header.e_shnum; i++) {
            //Récupération de l'en-tête
            sections[i].section=lire_une_section(file, header);
            //Récupération du nom
            sections[i].name=getSectionName(header, file,i);
        }
    }else{
        fprintf(stderr,"Erreur malloc sections");
        exit(1);
    }
    return sections;
}

void print_elf_section(Elf32Hdr header, Tab_Sec * sections, FILE* elfFile){

    printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%02x: \n\n", header.e_shnum, header.e_shoff);
    printf("En-têtes de section :\n");
    printf("%2s\t%-20s\t%-14s\t%-8s\t%-6s\t%-6s\t%-2s\t%-3s\t%-2s\t%-3s\t%-2s","[Nr]","Nom","Type","Adr","Décala.","Taille","ES","Fan","LN","Inf","Al");
    printf("\n");

    char* name = "";
    int * flag_ind = malloc(sizeof(int));
    for (size_t i = 0; i < header.e_shnum; i++)
    {
        //Affichage indice
        printf("[%2ld]\t",i);

        //Affichage nom
        name = sections[i].name;
        printf("%-20s\t",name);
        
        //Affichage type
        ELF32_Word type = sections[i].section.sh_type;
        char * typeAffich = get_section_type(type, name);
        printf("%-14s\t",typeAffich);
        
        //Affichage sh_addr
        printf("%08x\t",sections[i].section.sh_addr);

        //Affichage sh_offset
        printf("%06x\t",sections[i].section.sh_offset);

        //Affichage sh_size
        printf("%06x\t",sections[i].section.sh_size);

        //Affichage sh_entsize
        printf("%02x\t",sections[i].section.sh_entsize);

        //Affichage des flags
        ELF32_Word sh_flags = sections[i].section.sh_flags;
        char * flag = get_section_flag(sh_flags,flag_ind);

        while (*flag_ind >= 0)
        {
            printf("%c",flag[*flag_ind]);
            (*flag_ind)--;
        }
        free(flag);
        printf("\t");
      
        //Affichage sh_link
        printf("%-2d\t",sections[i].section.sh_link);

        //Affichage sh_info
        printf("%-3d\t",sections[i].section.sh_info);

        //Affichage sh_addralign
        printf("%-2d\t",sections[i].section.sh_addralign);

        
        printf("\n");
    }

    free(flag_ind);
    
    printf("Clé des fanions :\nW (écriture), A (allocation), X (exécution),M (fusion), S (chaînes), I (info),\nL (ordre des liens), O (traitement supplémentaire par l'OS requis),G (groupe),\nT (TLS), x (inconnu), o (spécifique à l'OS), E (exclu),\np (processor specific)");
    printf("\n\n");
}

char * getSectionName(Elf32Hdr header, FILE* elfFile, int i){

    Elf32_Shdr sectionStrTab;
    //On récupère la table des strings
    fseek(elfFile, header.e_shoff +header.e_shstrndx * header.e_shentsize, SEEK_SET);
    fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);

    //On récupère la liste des caractère de la table des strings
    char* SectNames =  malloc(reverse_endianess(sectionStrTab.sh_size,header,0));
    fseek(elfFile, reverse_endianess(sectionStrTab.sh_offset,header,0), SEEK_SET);
    fread(SectNames, 1, reverse_endianess(sectionStrTab.sh_size,header,0), elfFile);

    char* name = "";
    //On se place à la bonne section
    fseek(elfFile, header.e_shoff + i * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);

        // On récupère le nom
    if (sectionStrTab.sh_name){
        name = SectNames + reverse_endianess(sectionStrTab.sh_name,header,0);
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

