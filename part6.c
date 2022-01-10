#include "part6.h"

// initialiser 
int* init_tab_renum(Elf32Hdr header){
    int* tab= malloc(sizeof(int)* reverse_endianess(header.e_shnum, header, 1)) ;
    for(int i=0; i<reverse_endianess(header.e_shnum, header, 1); i++){
        tab[i]=i;
    }
    return tab;
}

Elf32_Shdr* renumerotation_table_section(Elf32Hdr header, Elf32_Shdr * sect, int * tab_renum){
	//on compte le nombre de sections de relocations 
	ELF32_Word type;
	int cpt_rel = 0;
	for (size_t i = 0; i < reverse_endianess(header.e_shnum, header, 1); i++){
		type = reverse_endianess(sect[i].sh_type, header, 0);
		if (type == 9){
			cpt_rel++; }
		}

	// on crée une nouvelle table des sections avec seulement les sections qui ne sont pas des relocations	
	Elf32_Shdr * sect2 = malloc(sizeof(Elf32_Shdr) * reverse_endianess(header.e_shnum, header, 1));
	int num=0;
	ELF32_Word type2 ;
	for (int i = 0; i < reverse_endianess(header.e_shnum, header, 1); i++){
		type2 = reverse_endianess(sect[i].sh_type, header, 0);
		if (type2 != 9){
			sect2[num] = sect[i]; 
			tab_renum[i] = num;
			num++; // on incrémente num que dans le if pour qu'il se décale du i quand on trouve une table de relocation
		}
	}
	return sect2;
}

void renumerotation_table_symbole(FILE* file,Elf32_Sym* tab_sym ,Elf32Hdr header,Elf32_Shdr* tab_sec, int* tab_renum){

	// on se sert de la table de renumérotation pour modifier la colonne Ndx de la table des symboles
	int taille = get_taille_table_symbole(file, tab_sec, header);
    // trier_table_symbole(tab_sym, header, tab_sec, file);    
	for (int i = 1; i<taille ; i++){
		tab_sym[i].st_shndx = reverse_endianess(tab_renum[reverse_endianess(tab_sym[i].st_shndx,header, 1)],header,1); }
}


void trier_table_symbole(Elf32_Sym* tab_sym,Elf32Hdr header,Elf32_Shdr* tab_sec,FILE* file){
    int taille = get_taille_table_symbole(file, tab_sec, header);
    int indice=1;
    for(int i=1; i<taille; i++){
        if((tab_sym[i].st_info)==STT_SECTION){
            Elf32_Sym temp = tab_sym[indice];
            tab_sym[indice]= tab_sym[i];
            tab_sym[i]= temp;
            indice++;
        }
    }
}

