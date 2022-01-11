#include "part6.h"

// initialiser 
int* init_tab_renum(Elf32Hdr header){
    int* tab= malloc(sizeof(int)* header.e_shnum) ;
    for(int i=0; i<header.e_shnum; i++){
        tab[i]=i;
    }
    return tab;
}
int nbFctReloc(Elf32Hdr * header, Tab_Sec* sect){
	ELF32_Word type;
	int cpt_rel = 0;
	for (size_t i = 0; i < header->e_shnum; i++){
		type = sect[i].section.sh_type;
		if (type == 9){
			cpt_rel++; 
			}
		}
	return cpt_rel;
	}

int nbSectVide(Elf32Hdr * header, Tab_Sec* sect){
	int cpt_vide = 0;
	for (size_t i = 1; i < header->e_shnum; i++){
		if (sect[i].section.sh_size==0){
			cpt_vide++; 
			}
		}
	return cpt_vide;
	}

Tab_Sec* renumerotation_table_section(Elf32Hdr * header, Tab_Sec* sect, int * tab_renum, int addrText, int addrData){
	//on compte le nombre de sections de relocations 
	int cpt_rel= nbFctReloc(header, sect);
	int cpt_vide= nbSectVide(header, sect);
	// on crée une nouvelle table des sections avec seulement les sections qui ne sont pas des relocations	
	Tab_Sec * sect2 = malloc(sizeof(Tab_Sec) * (header->e_shnum-cpt_rel-cpt_vide));
	int num=1;
	sect2[0]=sect[0];
	ELF32_Word type2 ;
	int textOffset=0;
	int indText=0;
	int indData=0;
	for (int i = 1; i < header->e_shnum; i++){
		type2 = sect[i].section.sh_type;
		if(strcmp(sect[i].name,".text")==0 && sect[i].section.sh_size!=0){
			sect2[num].section = sect[i].section; 
			sect2[num].name=sect[i].name;
			tab_renum[i] = num;
			sect2[num].section.sh_addr=addrText;
			sect2[num].section.sh_offset= sect2[num].section.sh_offset + addrText;
			textOffset=sect[i].section.sh_offset;
			indText=num;
			num++;	
		} else if(strcmp(sect[i].name,".data")==0 && sect[i].section.sh_size!=0){	
			sect2[num].section = sect[i].section; 
			sect2[num].name=sect[i].name;
			tab_renum[i] = num;
			sect2[num].section.sh_addr=addrData;
			sect2[num].section.sh_offset= textOffset + addrData;
			indData=num;
			num++;	
		} else if (type2 != 9 && sect[i].section.sh_size!=0 ) {	
		sect2[num].section = sect[i].section; 
		sect2[num].name=sect[i].name;
		tab_renum[i] = num;

		num++;	
		}

	}
	header->e_shnum=header->e_shnum-cpt_rel-cpt_vide;
	header->e_shstrndx=header->e_shstrndx-cpt_rel-cpt_vide;

	int aranges =0;
	int line=0;
	for(int i=0; i<header->e_shnum;i++){
		if(strcmp(sect2[i].name,".debug_aranges")==0){
			aranges=i;
		} else if(strcmp(sect2[i].name,".debug_line")==0){
			line=i;
		}
	}
	Tab_Sec temp=sect2[aranges];
	sect2[aranges]=sect2[line];
	sect2[line]=temp;
	if(indData==0){
		indData=indText;
	}
	for(int i=indData+1; i<header->e_shnum;i++){
		sect2[i].section.sh_offset= sect2[i-1].section.sh_offset+sect2[i-1].section.sh_size;
	}

	return sect2;
}

void renumerotation_table_symbole(FILE* file,Tab_Sym* tab_sym ,Elf32Hdr header,Tab_Sec* tab_sec, int* tab_renum){

	// on se sert de la table de renumérotation pour modifier la colonne Ndx de la table des symboles
	int taille = get_taille_table_symbole(file, tab_sec, header);
    trier_table_symbole(tab_sym, header, tab_sec, file);    
	for (int i = 1; i<taille ; i++){
		tab_sym[i].symbole.st_shndx = tab_renum[tab_sym[i].symbole.st_shndx]; 
		tab_sym[i].symbole.st_value=tab_sym[i].symbole.st_value+ tab_sec[tab_sym[i].symbole.st_shndx].section.sh_addr;
		}
}


void trier_table_symbole(Tab_Sym* tab_sym,Elf32Hdr header,Tab_Sec* tab_sec,FILE* file){
    int taille = get_taille_table_symbole(file, tab_sec, header);
    int indice=1;
    for(int i=1; i<taille; i++){
        if((tab_sym[i].symbole.st_info)==STT_SECTION){
            Tab_Sym temp = tab_sym[indice];
            tab_sym[indice]= tab_sym[i];
            tab_sym[i]= temp;
            indice++;
        }
    }
}

