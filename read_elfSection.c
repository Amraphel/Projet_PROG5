#include "read_elfSection.h"

int read_elf_section(FILE *elfFile, Elf32Hdr header)
{
    Elf32_Shdr section[header.e_shnum];

    //fseek(elfFile, header.e_shoff + header.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
    fseek(elfFile, header.e_shoff, SEEK_SET);

    printf("\nIl y a %d en-têtes de section, débutant à l'adresse de décalage 0x%02x: \n", header.e_shnum, header.e_shoff);

    printf("En-têtes de section :\n");

    printf("  [Nr]\tNom\tType\t\tAdr\tDécala.\tTaille\tES\tFan\tLN\tInf\tAl\n");

    for (size_t i = 0; i < header.e_shnum; i++)
    {
        fread(&section[i], 1, sizeof(Elf32_Shdr), elfFile);
    
        printf("[%ld]\t",i);
    
        //printf("%d\t", header.e_shstrndx[&section[i].sh_name]);
        //printf("%d\t", section[i].sh_name);

        // fseek(elfFile, header.e_shoff + header.e_shstrndx + section[], SEEK_SET);
        // char* stringTable = header.e_shstrndx;

        // char name = header.e_shstrndx + section[i].sh_name;
        // printf("%c\n",name);

        // for(i=0;i<header->e_shnum;i++) {
        //     char* name = stringTable + nameoffset;
        //     printf("%s\n",name);
        //     section++;
        // }   

        printf("DEFAULT\t");
        
    char * type = NULL;

        // Type
        switch (section[i].sh_type)
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

            printf("%20s",type);
        
        printf("%08x\t",section[i].sh_addr);
        printf("%06x\t",section[i].sh_offset);
        printf("%06x\t",section[i].sh_size);

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