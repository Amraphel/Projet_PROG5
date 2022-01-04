#include "read_relocation_table.h"

int read_reloc_table(FILE* file,Elf32Hdr header, Elf32_Shdr* sections, Elf32_Sym* symboles ){
    for(int i=0; i<header.e_shnum; i++){
        if(reverse_endianess(sections[i].sh_type)==4){
            int taille= reverse_endianess(sections[i].sh_size);
            int taillent= reverse_endianess(sections[i].sh_entsize);
            int nbentrees= taille/taillent;
            printf("Section de réadressage '%s' à l'adresse de décalage 0x%x contient %d entrées: \n", getSectionName(header,file,i), sections[i].sh_addr, nbentrees);
            printf("Décalage\t Info\t Type\t Val.-symboles\t Noms-symb.+ Addenda");
            fseek(file,(reverse_endianess(header.e_shoff) +i*(reverse_endianess(header.e_shentsize) >> 16)), SEEK_SET);
            for(int j=0; j<nbentrees; j++){
                Elf32_Rela rela;
                fread(&rela, 1, sizeof(Elf32_Rela), file);
                printf("%12x\t", reverse_endianess(rela.r_offset));
                printf("%12x\t", reverse_endianess(rela.r_info));
                switch ((reverse_endianess(rela.r_info)& 15))
                {
                case 0:
                    printf("NONE\t");
                    break;
                case 1:
                    printf("R_X86_64_32\t");
                    break;
                case 2:
                    printf("R_X86_64_PC32\t");
                    break;
                case 3:
                    printf("R_X86_64_GOT32\t");
                    break;
                case 4:
                    printf("R_X86_64_PLT32\t");                  
                    break;
                case 5:
                    printf("R_X86_64_COPY\t");
                    break;
                case 6:
                    printf("R_X86_64_GLOB_DAT\t");
                    break;
                case 7:
                    printf("R_X86_64_JUMP_SLO\t");
                    break;
                case 8:
                    printf("R_X86_64_RELATIVE\t");
                    break;
                case 9:
                    printf("R_X86_64_GOTOFF\t");
                    break;
                case 10:
                    printf("R_X86_64_GOTPC\t");
                    break;
                case 11:
                    printf("R_X86_64_32PLT\t");
                    break;
                
                default:
                    break;
                }

                if((reverse_endianess(rela.r_info)& 15)==8){
                    printf("            \t");
                } else { 
                    if((reverse_endianess(rela.r_info)& 15)==5){
                        printf("%12x\t", reverse_endianess(rela.r_offset));
                    }else {
                        printf("%12x\t", reverse_endianess(rela.r_addend));
                    }
                    // int nbSym= (reverse_endianess(rela.r_info)>>8);
                    // printf("%s + %d\n", symboles[nbSym].st_name, rela.r_addend);
                }
            }
            printf("\n");
        }
    }
}


int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1], "rb");
    if(!file){
        printf("erreur de lecture");
    } else {
        Elf32Hdr header = read_elf_header(file);
        Elf32_Shdr* section = read_elf_section(file, header);
        Elf32_Sym* symbole;
        read_reloc_table(file,header,section,symbole);
        free(section);
    }

    fclose(file);
    return 0;
}