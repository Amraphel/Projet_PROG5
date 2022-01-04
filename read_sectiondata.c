#include "read_elfSection.h"

int read_elf_section_data(char * a,Elf32Hdr header, Elf32_Shdr * sections, FILE* file){
    int test=0;
    int i=0;
    //On vérifie que la section existe bien
    while(i<header.e_shnum || test==0){
        if(a==sections[i].sh_name || a==i){
            test=1;
        }else{
            i++;
        }
    }
    //La section n'existe pas
    if(test==0){
        printf("AVERTISSEMENT: LA section %s n'a pas été vidangée parce qu'inexistante !", a);
    } else {
        //On vérifie que la section contient des données
        int taille = sections[i].sh_size;
        if(taille==0){
        printf("La section « %s » n'a pas de données à vidanger .", sections[i].sh_name);
    } else{
        printf("Vidange hexadécimale de la section « %s » :\n", sections[i].sh_name);
        //On se place à l'adresse de la section
        fseek(file,(sections[i].sh_addr+sections[i].sh_offset), SEEK_SET);
        //On stocke les données dans un tableau 
        uint8_t tab[taille];
        fread(tab, 1,taille*sizeof(uint8_t),file);
        for(int i =0; i<=taille/16; i++){
            //On affiche l'adresse suivie des données
            uint32_t addr = sections[i].sh_addr + (i<1);
            printf("  0x%08x ", addr);
            int j=0;
            while(j+i*16<taille && j<16){
                if(j%4==3){
                    printf("%02x ", tab[j+i*16]);
                } else{
                    printf("%02x", tab[j+i*16]);
                } 
                j++;
            }
            j=0;
            //On affiche les caractères correspondant
            while(j+i*16<taille && j<16){
                printf("%c", tab[j+i*16]);
            }

            printf("\n");
        }  
    }
    return 0;
    }
}






int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[2], "rb");
    if(!file){
        printf("erreur de lecture");
    } else {
        Elf32Hdr header = read_elf_header(file);
        Elf32_Shdr* section = read_elf_section(file, header);
        read_elf_section_data(argv[1],header,section,file);
    }

    fclose(file);
    return 0;
}