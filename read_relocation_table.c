#include "read_relocation_table.h"

int get_number_reloc_sect(Elf32Hdr header, Tab_Sec* sections){
    int i=0;
    for(int k=0; k<header.e_shnum; k++){
        if(sections[k].section.sh_type==9){
            i++;
        }
    }
    return i;
}

Tab_Rel* read_temp_reloc_table (FILE* file,Elf32Hdr header, Tab_Sec* sections){
    int nbReloc = get_number_reloc_sect(header, sections);
    if(nbReloc!=0) {
        //On alloue la future table
        Tab_Rel* tab_reloc = malloc(sizeof(Tab_Rel)*nbReloc);
        //L'indice où se place dans tab_reloc
        int indice=0;
        for(int k=0; k<header.e_shnum; k++){
            //Si la section est de type REL
            if(sections[k].section.sh_type==9){
                char i2[8];
                sprintf(i2, "%hu", k);
                //On récupère les données de la section
                uint8_t * tab_sec = read_elf_section_data(i2,header,sections,file);
                int taille= sections[k].section.sh_size;
                int taillent= sections[k].section.sh_entsize;
                //On récupère le nombre d'entrées dans la section
                int nbentrees= taille/taillent;
                //On stocke les données
                tab_reloc[indice].taille=nbentrees;
                tab_reloc[indice].indice=k;
                Elf32_Rel* list_rel= malloc(sizeof(Elf32_Rel)*nbentrees);
                //On récupère les différentes entrées
                for(int i=0; i<nbentrees; i++){
                    Elf32_Rel rel;
                    ELF32_Addr  offset=0;
                    ELF32_Word  info=0;
                    for(int j=0; j<8; j++){
                        if(j<4){
                            offset += tab_sec[i*8+j]<<(24-((j%4)*8));
                        } else{
                            info += tab_sec[i*8+j]<<(24-((j%4)*8));
                        }
                    }
                    rel.r_info=info;
                    rel.r_offset=offset;
                    list_rel[i]=rel;
                    
                }
                tab_reloc[indice].list_rel=list_rel;
                indice++;
            }

        }
        return tab_reloc;
    } else{
        return NULL;
    }
}


void print_reloc_table(Tab_Rel* tab, Elf32Hdr header, Tab_Sec* sections, FILE * file, Tab_Sym* sym){
    int nbReloc = get_number_reloc_sect(header, sections);
    if(nbReloc==0){
        printf("Il n'y a pas de réadressage dans ce fichier\n");
    } else {
        for(int i=0; i<nbReloc; i++){
            printf("Section de réadressage '%s' à l'adresse de décalage 0x%x contient %d entrées: \n", getSectionName(header,file,tab[i].indice), sections[tab[i].indice].section.sh_offset, tab[i].taille);
            printf("%-8s %-8s %-12s %-15s %-12s\n","Décalage", "Info",  "Type", "Val.-sym",   "Noms-symboles");
            for(int j=0; j<tab[i].taille; j++){
                
                printf("%08x ",tab[i].list_rel[j].r_offset);
                printf("%08x ", tab[i].list_rel[j].r_info);
                char * type;
                switch (ELF32_R_TYPE(tab[i].list_rel[j].r_info))
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

                printf("%08x%7s", sym[ELF32_R_SYM(tab[i].list_rel[j].r_info)].symbole.st_value,"");
                if(ELF32_ST_TYPE(sym[ELF32_R_SYM(tab[i].list_rel[j].r_info)].symbole.st_info)==STT_SECTION){
                printf("%-15s", getSectionName(header,file, sym[ELF32_R_SYM(tab[i].list_rel[j].r_info)].symbole.st_shndx));
                } else{
                    unsigned char * nom =sym[ELF32_R_SYM(tab[i].list_rel[j].r_info)].name;
                    printf("%-15s",nom);
                }

                printf("\n");
            }
            printf("\n");    
        }
        
    }
}


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