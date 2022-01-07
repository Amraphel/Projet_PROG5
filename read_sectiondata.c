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
            fseek(file,(reverse_endianess(header.e_shoff,header, 0) +indice_sect_data*(reverse_endianess(header.e_shentsize,header, 1) )), SEEK_SET);
            //On stocke les données dans un tableau 
            uint8_t* tab = malloc(sizeof(uint8_t)*taille_sect_data);
            fread(&sec, 1, sizeof(Elf32_Shdr), file);
            
            fseek(file, reverse_endianess(sec.sh_offset,header, 0), SEEK_SET);
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
    while(indice_sect_data<reverse_endianess(header.e_shnum,header, 0) && test_sect_data==-1){
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
    return reverse_endianess(sections[indice_sect_data].sh_size,header, 0);
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
            uint32_t addr = reverse_endianess(sections[indice].sh_addr,header, 0) ;
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