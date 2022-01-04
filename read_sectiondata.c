#include "read_sectiondata.h"

char * getSectionName(Elf32Hdr header, FILE* elfFile, int i){

    Elf32_Shdr sectionStrTab;

    fseek(elfFile, reverse_endianess(header.e_shoff) + (reverse_endianess(header.e_shstrndx) >> 16) * (reverse_endianess(header.e_shentsize) >> 16), SEEK_SET);
    fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);
    char* SectNames = NULL;
    SectNames = malloc(reverse_endianess(sectionStrTab.sh_size));
    fseek(elfFile, reverse_endianess(sectionStrTab.sh_offset), SEEK_SET);
    fread(SectNames, 1, reverse_endianess(sectionStrTab.sh_size), elfFile);

    char* name = "";
    //section name
    fseek(elfFile, reverse_endianess(header.e_shoff) + i * sizeof(Elf32_Shdr), SEEK_SET);
    fread(&sectionStrTab, 1, sizeof(Elf32_Shdr), elfFile);

        // print section name
    if (reverse_endianess(sectionStrTab.sh_name));
    name = SectNames + reverse_endianess(sectionStrTab.sh_name);
    return name;
}

int read_elf_section_data(char * a,Elf32Hdr header, Elf32_Shdr * sections, FILE* file){
    printf("\n");
    int test=0;
    int i=0;
    char* i2;
    Elf32_Shdr sec;
    printf("%s", i2);
    //On vérifie que la section existe bien
    while(i<reverse_endianess(header.e_shnum) && test==0){
        if(strcmp(a, getSectionName(header,file,i))==0 || strcmp(a,i2)==0){
            test=1;
        }else{
            i++;
            sprintf(i2, "%d", i);
        }
    }
    //La section n'existe pas
    if(test==0){
        printf("AVERTISSEMENT: LA section %s n'a pas été vidangée parce qu'inexistante !\n", a);
    } else {
        //On vérifie que la section contient des données
        int taille = reverse_endianess(sections[i].sh_size);
        if(taille==0){
        printf("La section « %s » n'a pas de données à vidanger .\n", getSectionName(header,file,i));
    } else{
        printf("Vidange hexadécimale de la section « %s » :\n",  getSectionName(header,file,i));
        //On se place à l'adresse de la section
        fseek(file,(reverse_endianess(header.e_shoff) +i*(reverse_endianess(header.e_shentsize) >> 16)), SEEK_SET);
        //On stocke les données dans un tableau 
        uint8_t tab[taille];
        fread(&sec, 1, sizeof(Elf32_Shdr), file);
        uint32_t addr = reverse_endianess(sections[i].sh_addr) ;
        fseek(file, reverse_endianess(sec.sh_offset), SEEK_SET);
        fread(tab, 1,taille,file);
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
        Elf32_Shdr * sect;
        sect = read_elf_section(file, header);

        read_elf_section_data(argv[1],header,sect,file);
        free(sect);

    }

    fclose(file);
    return 0;
}