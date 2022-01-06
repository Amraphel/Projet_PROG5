#include "read_relocation_table.h"
// int nbsection;
// int* tab_ent;

void read_temp_reloc_table (FILE* file,Elf32Hdr header, Elf32_Shdr* sections, Elf32_Sym* sym){
    int test=0;
    for(unsigned short k=0; k<reverse_endianess(header.e_shnum,header,1); k++){
        if(reverse_endianess(sections[k].sh_type,header,0)==9){
            test=1;
            int taille= reverse_endianess(sections[k].sh_size,header,0);
            int taillent= reverse_endianess(sections[k].sh_entsize,header,0);
            int nbentrees= taille/taillent;
            // Elf32_Rel* tab_rel= malloc(sizeof(Elf32_Rela));
            char* i2= malloc(sizeof(char*));
            sprintf(i2, "%hu", k);
            uint8_t * tab_sec = read_elf_section_data(i2,header,sections,file);
            printf("Section de réadressage '%s' à l'adresse de décalage 0x%x contient %d entrées: \n", getSectionName(header,file,k), reverse_endianess(sections[k].sh_offset,header,0), nbentrees);
            printf("%-8s %-8s %-12s %-15s %-12s\n","Décalage", "Info",  "Type", "Val.-sym",   "Noms-symboles");
            for(int i=0; i<nbentrees; i++){
                // Elf32_Rel rel;
                ELF32_Addr  offset=0;
                ELF32_Word  info=0;
                for(int j=0; j<8; j++){
                    if(j<4){
                        offset += tab_sec[i*8+j]<<(24-((j%4)*8));
                        // printf("offset = %012x\n", offset);
                    } else{
                        info += tab_sec[i*8+j]<<(24-((j%4)*8));
                    }
                }
                // rel.r_info=info;
                // rel.r_offset=offset;
                // tab_rel[i]=rel;
                // if(reverse_endianess(header.e_type, header, 1)==0x1){
                //     offset= rela.r_offset+ sections[i].sh_addr;
                // } else {
                // }
                printf("%08x ",offset);
                printf("%08x ", info);
                char * type;
                switch (ELF32_R_TYPE(info))
                {
                case 0:
                    type="R_ARM_NONE";
                    break;

                case 2:
                    type="R_ARM_ABS32";
                    break;

                case 28:
                    type="R_ARM_CALL";                  
                    break;
                case 29:
                    type="R_ARM_JUMP24";
                    break;
                case 5:
                    type="R_ARM_ABS16";
                    break;
                case 8:
                    type="R_ARM_ABS8";
                    break;
                default:
                    type="default";
                    break;
                }
                printf("%-12s ", type);

                    // fprintf(stderr," boucle i : %i",i);
                    // fprintf(stderr," boucle i : %08x",sym[1].st_value);
                    printf("%08x%7s", reverse_endianess(sym[ELF32_R_SYM(info)].st_value, header,0),"");
                    if(ELF32_ST_TYPE(sym[ELF32_R_SYM(info)].st_info)==STT_SECTION){
                        printf("%-15s", getSectionName(header,file, reverse_endianess(sym[ELF32_R_SYM(info)].st_shndx,header,1)));
                    } else{
                        unsigned char * nom =renvoyer_nom_du_symbole(ELF32_R_SYM(info), file,header, sections);
                        printf("%-15s",nom);
                    }
                    // int nbSym= (reverse_endianess(rela.r_info, header, 0)>>8);
                    // printf("%s\n", );

                printf("\n");
            }
            printf("\n");
            
            free(i2);

         }
    }
    if(test==0){
        printf("\nIl n'y a pas de réadressages dans ce fichier\n");
    }
}

// Elf32_Rel** read_reloc_table(FILE* file,Elf32Hdr header, Elf32_Shdr* sections){
//     Elf32_Rel** table=malloc(sizeof(Elf32_Rel*));
//     int k=0;
//     tab_ent= malloc(sizeof(int)*header.e_shnum);
//     for(int i=0; i<header.e_shnum; i++){
//         if(reverse_endianess(sections[i].sh_type,header,0)==9){
//             int taille= reverse_endianess(sections[i].sh_size,header,0);
//             int taillent= reverse_endianess(sections[i].sh_entsize,header,0);
//             int nbentrees= taille/taillent;
//             Elf32_Rel* tab_rel= malloc(sizeof(Elf32_Rela));
//             char* i2= malloc(sizeof(char*));
//             sprintf(i2, "%d", i);
//             uint8_t * tab_sec = read_elf_section_data(i2,header,sections,file);
//             for(int i=0; i<nbentrees; i++){
//                 Elf32_Rel rel;
//                 ELF32_Addr  offset=0;
//                 ELF32_Word  info=0;
//                 for(int j=0; j<8; j++){
//                     if(j<4){
//                         offset += tab_sec[i+j]<<(24-((j%4)*8));
//                     } else{
//                         offset += tab_sec[i+j]<<(24-((j%4)*8));
//                     }
//                 }
//                 rel.r_info=info;
//                 rel.r_offset=offset;
//                 tab_rel[i]=rel;
//             }
//             table[k]=tab_rel;
//             tab_ent[k]=nbentrees;
//             free(i2);
//         } else {
//             table[k]=NULL;
//         }
//         k++;
//     }
//     nbsection=k;
// }


// void print_sect_rel(FILE* file, Elf32Hdr header, Elf32_Shdr* sections,Elf32_Rel** tab, int nbsection, int* tab_entr){
//     for(int i=0; i<nbsection;i++){
//         if(tab[i]){
//             printf("Section de réadressage '%s' à l'adresse de décalage 0x%x contient %d entrées: \n", getSectionName(header,file,i), sections[i].sh_addr, tab_ent[i]);
//             printf("%-12s %-12s %-15s %-12s %-12s","Décalage", "Info",  "Type", "Val.-symboles",   "Noms-symb.\n");

//             for(int j=0; j<tab_entr[i]; j++){
//                 Elf32_Rel rela;
//                 fread(&rela, 1, sizeof(Elf32_Rela), file);
//                 ELF32_Addr offset;
//                 // if(reverse_endianess(header.e_type, header, 1)==0x1){
//                 //     offset= rela.r_offset+ sections[i].sh_addr;
//                 // } else {
//                     offset= rela.r_offset;
//                 // }
//                 printf("%012x ",offset);
//                 printf("%012x ", rela.r_info);
//                 switch (ELF32_R_TYPE(rela.r_info))
//                 {
//                 case 0:
//                     printf("NONE ");
//                     break;
//                 case 1:
//                     printf("R_X86_64_32 ");
//                     break;
//                 case 2:
//                     printf("R_X86_64_PC32 ");
//                     break;
//                 case 3:
//                     printf("R_X86_64_GOT32 ");
//                     break;
//                 case 4:
//                     printf("R_X86_64_PLT32 ");                  
//                     break;
//                 case 5:
//                     printf("R_X86_64_COPY ");
//                     break;
//                 case 6:
//                     printf("R_X86_64_GLOB_DAT ");
//                     break;
//                 case 7:
//                     printf("R_X86_64_JUMP_SLO ");
//                     break;
//                 case 8:
//                     printf("R_X86_64_RELATIVE ");
//                     break;
//                 case 9:
//                     printf("R_X86_64_GOTOFF ");
//                     break;
//                 case 10:
//                     printf("R_X86_64_GOTPC ");
//                     break;
//                 case 11:
//                     printf("R_X86_64_32PLT ");
//                     break;
//                 default:
//                     printf("default     ");
//                     break;
//                 }

//                 if((rela.r_info& 15)==8){
//                     printf("            \t");
//                 } else { 
//                     printf("%012x\t", rela.r_offset);
//                     // int nbSym= (reverse_endianess(rela.r_info, header, 0)>>8);
//                     // printf("%s\n", );
//                 }
//                 printf("\n");
//             }
//             printf("\n");
//             }
        
//     }
    
// }
    
// void liberer_sect_rel(Elf32_Rel** tab, int nbsection){
//     for(int i=0; i<nbsection; i++){
//         free(tab[i]);
//     }
//     free(tab);
// }


// int main(int argc, char *argv[])
// {
//     FILE *file = fopen(argv[1], "rb");
//     if(!file){
//         printf("erreur de lecture");
//     } else {
//         Elf32Hdr header = read_elf_header(file);
//         Elf32_Shdr* section = read_elf_section(file, header);
//         Elf32_Sym* sym= renvoyer_table_sym(file, header, section);
//         read_temp_reloc_table(file,header,section, sym);
//         // print_sect_rel(file,header,section,tab,nbsection,tab_ent);
//         // liberer_sect_rel(tab, nbsection);

//         // free(sym);
//         free(section);
//     }

//     fclose(file);
//     return 0;
// }