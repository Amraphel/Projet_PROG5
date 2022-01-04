#include "read_elfSection.h"

int read_elf_section(FILE *elfFile, Elf32Hdr header)
{
    /*
    Elf32_Shdr section;

    fseek(elfFile, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
    fread(&section, 1, sizeof(section), elfFile);
    char* SectNames = NULL;
    SectNames = malloc(section.sh_size);
    fseek(elfFile, section.sh_offset, SEEK_SET);
    fread(SectNames, 1, section.sh_size, elfFile);

    for (int idx = 0; idx < header.e_shnum; idx++){
        char* name = "";

        fseek(elfFile, header.e_shoff + idx * sizeof(section), SEEK_SET);
        fread(&section, 1, sizeof(section), elfFile);

        // print section name
        if (section.sh_name);
        name = SectNames + section.sh_name;
            
        printf("%i %s\n", idx, name);
    }
    */

    Elf32_Shdr section[header.e_shnum];
    // char* stringTable = NULL;

    printf("\nIl y a %d en-têtes de section, débutant à l'adresse de décalage 0x%02x: \n", reverse_endianess(header.e_shnum) >> 16, reverse_endianess(header.e_shoff));
    printf("En-têtes de section :\n");
    printf("  [Nr]\tNom\tType\t  Adr\tDécala.\tTaille\tES\tFan\tLN\tInf\tAl\n");

    // fseek(elfFile, reverse_endianess(header.e_shoff) + (reverse_endianess(header.e_shstrndx) >> 16), SEEK_SET);
    // fread(&stringTable, 1, sizeof(Elf32_Shdr), elfFile);

    fseek(elfFile, reverse_endianess(header.e_shoff), SEEK_SET);

    for (size_t i = 0; i < reverse_endianess(header.e_shnum) >> 16; i++) {
        fread(&section[i], 1, sizeof(Elf32_Shdr), elfFile);
    }

    char * type = NULL;


    for (size_t i = 0; i < reverse_endianess(header.e_shnum) >> 16; i++)
    {
        printf("  [%ld]\t",i);

        // printf("%4d '%s'\n", reverse_endianess(section[i].sh_name),
        //     stringTable + reverse_endianess(section[i].sh_name));

        if (i == (reverse_endianess(header.e_shnum) >> 16) -1)
        {
            printf("%x",section[i].sh_name);
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

            printf("%s       ",type);
        
        printf("%08x ",reverse_endianess(section[i].sh_addr));
        printf("%06x ",reverse_endianess(section[i].sh_offset));
        printf("%06x ",reverse_endianess(section[i].sh_size));
        printf("%02x ",reverse_endianess(section[i].sh_entsize));

        char flag[4];
        int flag_ind = -1;
        int n = 15;
        int j = n-1;
        long int tmp;
        long int listFlag[15] = { 0x1, 0x2, 0x4, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x0ff00000, 0x10000000, 0x40000000, 0x80000000, 0xf0000000};
        long int flagValue;
        flagValue = reverse_endianess(section[i].sh_flags);
        if(flagValue){
            // printf("\n");
            while(flagValue > 0 && j >= 0){
                // printf("listFlag %li   flagValue %li",listFlag[j], flagValue);
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
        }else{
            printf("  ");
        }


        printf("%2d ",reverse_endianess(section[i].sh_link));
        printf("%2d ",reverse_endianess(section[i].sh_info));
        printf("%2d ",reverse_endianess(section[i].sh_addralign));



        printf("\n");
    }

    return 0;
}

int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "rb");
    if(!file){
        printf("erreur de lecture");
    } else {
        Elf32Hdr header = read_elf_header(file);
        read_elf_section(file, header);
    }

    fclose(file);
    return 0;
}