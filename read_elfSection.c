#include "read_elfSection.h"

Elf32_Shdr * read_elf_section(FILE *elfFile, Elf32Hdr header)
{
    Elf32_Shdr * section = NULL;
    section = malloc(sizeof(Elf32_Shdr) * (reverse_endianess(header.e_shnum) >> 16));

    printf("\nIl y a %d en-têtes de section, débutant à l'adresse de décalage 0x%02x: \n", reverse_endianess(header.e_shnum) >> 16, reverse_endianess(header.e_shoff));
    printf("En-têtes de section :\n");
    printf("%2s\t%12s\t%9s\t%8s\t%6s\t%6s\t%2s\t%3s\t%2s\t%3s\t%2s","[Nr]","Nom","Type","Adr","Décala.","Taille","ES","Fan","LN","Inf","Al"); //\t  Adr\tDécala.\tTaille\tES\tFan\tLN\tInf\tAl\n
    printf("\n");

    fseek(elfFile, reverse_endianess(header.e_shoff), SEEK_SET);



    for (size_t i = 0; i < reverse_endianess(header.e_shnum) >> 16; i++) {
        fread(&section[i], 1, sizeof(Elf32_Shdr), elfFile);
    }

    //section name
    Elf32_Shdr sectionStrTab;

    fseek(elfFile, reverse_endianess(header.e_shoff) + (reverse_endianess(header.e_shstrndx) >> 16) * (reverse_endianess(header.e_shentsize) >> 16), SEEK_SET);
    fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);
    char* SectNames = NULL;
    SectNames = malloc(reverse_endianess(sectionStrTab.sh_size));
    fseek(elfFile, reverse_endianess(sectionStrTab.sh_offset), SEEK_SET);
    fread(SectNames, 1, reverse_endianess(sectionStrTab.sh_size), elfFile);

    char* name = "";

    char * type = NULL;


    for (size_t i = 0; i < reverse_endianess(header.e_shnum) >> 16; i++)
    {
        //number
        printf("[%2ld]\t",i);

        //section name
        fseek(elfFile, reverse_endianess(header.e_shoff) + i * sizeof(Elf32_Shdr), SEEK_SET);
        fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);

        // print section name
        if (reverse_endianess(sectionStrTab.sh_name));
        name = SectNames + reverse_endianess(sectionStrTab.sh_name);

        if (reverse_endianess(sectionStrTab.sh_name)){
            printf("%12s\t",name);
        }else{
            printf("%12s\t","");
        }

        
        // Type
        switch (reverse_endianess(section[i].sh_type))
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
                type = "default!";
                break;
            }

        printf("%9s\t",type);
        
        printf("%08x\t",reverse_endianess(section[i].sh_addr));
        printf("%06x\t",reverse_endianess(section[i].sh_offset));
        printf("%06x\t",reverse_endianess(section[i].sh_size));
        printf("%02x\t",reverse_endianess(section[i].sh_entsize));

        char flag[4];
        int flag_ind = -1;
        int n = 15;
        int j = n-1;
        long int tmp;
        long int listFlag[15] = { 0x1, 0x2, 0x4, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x0ff00000, 0x10000000, 0x40000000, 0x80000000, 0xf0000000};
        long int flagValue;
        flagValue = reverse_endianess(section[i].sh_flags);
        if(flagValue){
            while(flagValue > 0 && j >= 0){
                if(listFlag[j] <= flagValue){
                    flag_ind++;
                    flagValue = flagValue - listFlag[j];
                    tmp = listFlag[j];

                    switch (tmp)
                    {
                        case 0x1:
                            flag[flag_ind] = 'W';
                            break;
                        case 0x2:
                            flag[flag_ind] = 'A';
                            break;
                        case 0x4:
                            flag[flag_ind] = 'X';
                            break;
                        case 0x10:
                            flag[flag_ind] = 'M';
                            break;
                        case 0x20:
                            flag[flag_ind] = 'S';
                            break;
                        case 0x40:
                            flag[flag_ind] = 'I';
                            break;
                        case 0x80:
                            flag[flag_ind] = 'L';
                            break;
                        case 0x100:
                            flag[flag_ind] = 'O';
                            break;
                        case 0x200:
                            flag[flag_ind] = 'G';
                            break;
                        case 0x400:
                            flag[flag_ind] = 'T';
                            break;
                        case 0x0ff00000:
                            flag[flag_ind] = 'o';
                            break;
                        case 0x10000000:
                            flag[flag_ind] = ' '; //manquant
                            break;
                        case 0x40000000:
                            flag[flag_ind] = ' '; //manquant
                            break;
                        case 0x80000000:
                            flag[flag_ind] = 'E';
                            break;
                        case 0xf0000000:
                            flag[flag_ind] = 'p';
                            break;
                        default:
                            flag[flag_ind] = 'x';
                            break;
                    }
                }
                j--;
                
            }
            
            while (flag_ind >= 0)
            {
                printf("%c",flag[flag_ind]);
                flag_ind--;
            }
            printf("\t");
        }else{
            printf("%3s\t","");
        }

        printf("%2d\t",reverse_endianess(section[i].sh_link));
        printf("%3d\t",reverse_endianess(section[i].sh_info));
        printf("%2d\t",reverse_endianess(section[i].sh_addralign));

        
        printf("\n");
    }

    free(SectNames);
    
    printf("Clé des fanions :\nW (écriture), A (allocation), X (exécution),\nM (fusion), S (chaînes), I (info),L (ordre des liens), O (traitement supplémentaire par l'OS requis),\nG (groupe),T (TLS), C (compressé), x (inconnu), o (spécifique à l'OS), E (exclu),y (purecode), p (processor specific)");
    printf("\n");

    return section;
}
/*
int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "rb");
    if(!file){
        printf("erreur de lecture");
    } else {
        Elf32Hdr header = read_elf_header(file);
        Elf32_Shdr * sect;
        sect = read_elf_section(file, header);
        free(sect);
    }

    fclose(file);
    return 0;
}*/