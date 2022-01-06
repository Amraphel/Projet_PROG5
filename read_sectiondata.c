#include "read_sectiondata.h"
int taille_sect_data;
int test_sect_data;
unsigned short indice_sect_data;

uint8_t* read_elf_section_data(char * a,Elf32Hdr header, Elf32_Shdr * sections, FILE* file){
    printf("\n");
    test_sect_data=0;
    indice_sect_data=0;
    char* i2= malloc(sizeof(char*));
    Elf32_Shdr sec;
    sprintf(i2, "%hu", indice_sect_data);
    //On vérifie que la section existe bien
    while(indice_sect_data<reverse_endianess(header.e_shnum,header, 0) && test_sect_data==0){
        if(strcmp(a, getSectionName(header,file,indice_sect_data))==0 || strcmp(a,i2)==0){
            test_sect_data=1;
        }else{
            indice_sect_data++;
            sprintf(i2, "%hu", indice_sect_data);
        }
    }
    free(i2);
    //La section n'existe pas
    if(test_sect_data==0){
        return NULL;
    } else {
        //On vérifie que la section contient des données
        taille_sect_data = reverse_endianess(sections[indice_sect_data].sh_size,header, 0);
        if(taille_sect_data==0){
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




void print_sectiondata(FILE* file,char * a, uint8_t * tab, Elf32Hdr header, Elf32_Shdr* sections, int taille, int test, int i) {
    if(test==0){
        printf("AVERTISSEMENT: La section %s n'a pas été vidangée parce qu'inexistante !\n", a);
    }else {
        if(taille==0){
        printf("La section « %s » n'a pas de données à vidanger .\n", getSectionName(header,file,i));
        } else{
            printf("Vidange hexadécimale de la section « %s » :\n",  getSectionName(header,file,i));
            uint32_t addr = reverse_endianess(sections[i].sh_addr,header, 0) ;
            for(int i =0; i<=taille/16; i++){
                //On affiche l'adresse suivie des données
                printf("  0x%08x ", addr);
                int j=0;
                while(j+i*16<taille && j<16){
                    printf("%02x", tab[j+i*16]);
                    if(j%4==3){
                    printf(" ");
                    } 
                    j++;
                }
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