#include "read_sectiondata.h"

uint8_t* read_elf_section_data(char * name,Elf32Hdr header, Elf32_Shdr * sections, FILE* file){
    Elf32_Shdr sec;
    //On vérifie que la section existe bien
    
    //La section n'existe pas
    int indice_sect_data = verify_sect_data(name, file, header);
    if(indice_sect_data==-1){
        return NULL;
    } else {
        //On vérifie que la section contient des données
        int taille_sect_data =verify_taille_sect(sections, indice_sect_data, header);
        if(verify_taille_sect(sections, indice_sect_data, header)==0){
            return NULL;
        } else{
            //On se place à l'adresse de la section
            fseek(file,(header.e_shoff +indice_sect_data*header.e_shentsize ), SEEK_SET);
            //On stocke les données dans un tableau 
            uint8_t* tab = malloc(sizeof(uint8_t)*taille_sect_data);
            sec = lire_une_section(file, header);
            
            fseek(file, sec.sh_offset, SEEK_SET);
            fread(tab, 1,taille_sect_data,file);
            return tab;
        }   
    }    
}

int verify_sect_data(char * name,FILE* file, Elf32Hdr header){
    char i2[8];
    int indice_sect_data=0;
    sprintf(i2, "%hu", indice_sect_data);
    int test_sect_data=-1;
    while(indice_sect_data<header.e_shnum && test_sect_data==-1){
        if(strcmp(name, getSectionName(header,file,indice_sect_data))==0 || strcmp(name,i2)==0){
            test_sect_data=1;
        }else{
            indice_sect_data++;
            sprintf(i2, "%hu", indice_sect_data);
        }
    }
    if(test_sect_data==-1){
        return test_sect_data;
    } else {
        return indice_sect_data;
    }

}

int verify_taille_sect(Elf32_Shdr* sections, int indice_sect_data, Elf32Hdr header){
    return sections[indice_sect_data].sh_size;
}


void print_sectiondata(FILE* file,char * name, uint8_t * tab, Elf32Hdr header, Elf32_Shdr* sections) {
    int indice=verify_sect_data(name,file,header);
    if(indice==-1){
        printf("AVERTISSEMENT: La section %s n'a pas été vidangée parce qu'inexistante !\n", name);
    }else {
        int taille = verify_taille_sect(sections, indice, header);
        if(taille==0){
        printf("La section « %s » n'a pas de données à vidanger .\n", getSectionName(header,file,indice));
        } else{
            printf("Vidange hexadécimale de la section « %s » :\n",  getSectionName(header,file,indice));
            uint32_t addr =sections[indice].sh_addr ;
            for(int i =0; i<=taille/16; i++){
                //On affiche l'adresse suivie des données
                printf("  0x%08x ", addr);
                int j=0;
                //On affiche les éléments du tableau
                while(j+i*16<taille && j<16){
                    printf("%02x", tab[j+i*16]);
                    if(j%4==3){
                    printf(" ");
                    } 
                    j++;
                }
                //On complète par des espaces pour aligner les éléments
                while(j<16){
                    printf("  ");
                    if(j%4==3){
                        printf(" ");
                    } 
                    j++;
                }
                j=0;
                //On affiche les caractères correspondant
                while(j+i*16<taille && j<16){
                    if((tab[j+i*16]-31)>0 && (tab[j+i*16]-128)<0){
                        printf("%c", tab[j+i*16]);
                    } else {
                        printf(".");
                    }
                    j++;
                }
                printf("\n");
                addr += 16;
            } 
             
        }

    }
}


            







// int main(int argc, char *argv[])
// {
//     FILE *file = fopen(argv[2], "rb");
//     if(!file){
//         printf("erreur de lecture");
//     } else {
//         Elf32Hdr header = read_elf_header(file);
//         Elf32_Shdr * sect;
//         sect = read_elf_section(file, header);
//         read_elf_section_data(argv[1],header,sect,file);
//         free(sect);

//     }

//     fclose(file);
//     return 0;
// }