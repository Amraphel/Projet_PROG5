#include "reimplantation_ARM.h"


void r_arm_jump(FILE* file,uint8_t* tab, Elf32Hdr header, Tab_Rel* tab_reloc, Tab_Sec* tab_sec,Tab_Sec* new_tab_sec,Tab_Sym* sym, int ind_sect, int ind_sect_reloc, int ind_reloc, int* tab_renum) {
	
	//On récupère le numéro du symbole et la valeur correspondante
	uint16_t addrSymbole =ELF32_R_SYM(tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_info);
	uint32_t valeurSymb = (uint32_t) sym[addrSymbole].symbole.st_value;

	//On récupère la valeur déjà présente dans le fichier, à l'emplacement que l'on doit modifier
	fseek(file,tab_sec[ind_sect].section.sh_offset + tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_offset, SEEK_SET);
	uint32_t addend;
	fread(&addend,1,sizeof(uint32_t), file);
	addend= reverse_endianess(addend,header,0);

	//On applique le calcul lié au type de réadressage
	uint32_t res = valeurSymb;
	res = (res - (new_tab_sec[tab_renum[ind_sect]].section.sh_addr + tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_offset))>>2;
	res = (addend & 0xff000000) + ((addend+res) & 0xffffff);
	// fseek(file, tab_sec[ind_sect].section.sh_offset+tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_offset, SEEK_SET);
	// fwrite(&res,1,sizeof(uint32_t),file);

	//On modifie la table de données de la section conernée
	rewrite_tab(tab, res, tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_offset);
}



void r_arm_abs32(FILE* file,uint8_t* tab, Elf32Hdr header, Tab_Rel* tab_reloc, Tab_Sec* tab_sec,Tab_Sym* sym, int ind_sect, int ind_sect_reloc, int ind_reloc) {
	//On récupère le numéro du symbole et la valeur correspondante
	uint16_t addrSymbole =ELF32_R_SYM(tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_info);
	uint32_t valeurSymb = (uint32_t) sym[addrSymbole].symbole.st_value;

	//On récupère la valeur déjà présente dans le fichier, à l'emplacement que l'on doit modifier
	fseek(file,tab_sec[ind_sect].section.sh_offset + tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_offset, SEEK_SET);
	uint32_t addend;
	fread(&addend,1,sizeof(uint32_t), file);
	addend= reverse_endianess(addend,header,0);
	
	//On applique le calcul lié au type de réadressage
	uint32_t res = valeurSymb + addend;

	// fseek(file, tab_sec[ind_sect].section.sh_offset+tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_offset, SEEK_SET);
	// fwrite(&res,1,sizeof(uint32_t),file);

	//On modifie la table de données de la section conernée
	rewrite_tab(tab, res, tab_reloc[ind_sect_reloc].list_rel[ind_reloc].r_offset);
}

void rewrite_tab(uint8_t* tab, uint32_t val, uint32_t offset){
	for(int i=0; i<4; i++){
		tab[offset+i]= (val & (0xff <<(24-8*i))) >>(24-8*i);
	}
}


Table_sect_data * write_reimp(FILE* file, Elf32Hdr header, Tab_Rel* tab_reloc, Tab_Sec* sect,Tab_Sec* new_tab_sec,Tab_Sym* sym, int* tab_renum){
	int ind_sect_reloc=0;
	int nbReloc = get_number_reloc_sect(header, sect);
	Table_sect_data* tab_sect_mod= malloc(sizeof(Table_sect_data)* nbReloc);
	for(int k=0; k<header.e_shnum; k++){
		//On cherche les section de réadressage
		if(sect[k].section.sh_type==9){
			//On récupère l'indice de la section à modifier
			int ind_sect= sect[k].section.sh_info;
			char * char_sect= malloc(sizeof(char)*8);
			sprintf(char_sect, "%d", ind_sect);
			uint8_t* tab= read_elf_section_data(char_sect, header,sect, file);
			//On applique les différents type de réadressage
			for(int i=0; i<tab_reloc[ind_sect_reloc].taille; i++){
					if(ELF32_R_TYPE(tab_reloc[ind_sect_reloc].list_rel[i].r_info)==2 ||
						ELF32_R_TYPE(tab_reloc[ind_sect_reloc].list_rel[i].r_info)==5 ||
						ELF32_R_TYPE(tab_reloc[ind_sect_reloc].list_rel[i].r_info)==8  ){
						r_arm_abs32( file,tab, header, tab_reloc, sect,sym, ind_sect,ind_sect_reloc, i);
					}else if(ELF32_R_TYPE(tab_reloc[ind_sect_reloc].list_rel[i].r_info)==28 || ELF32_R_TYPE(tab_reloc[ind_sect_reloc].list_rel[i].r_info)==29 ){
						r_arm_jump( file,tab, header, tab_reloc, sect,new_tab_sec,sym, ind_sect,ind_sect_reloc, i, tab_renum);
					}
			}

			tab_sect_mod[ind_sect_reloc].donnee= tab;
			tab_sect_mod[ind_sect_reloc].name=char_sect;
			ind_sect_reloc++;
			
		} 
	}
	return tab_sect_mod;
}


void print_sect_mod(FILE * file, Table_sect_data* tab, Elf32Hdr header, Tab_Sec* sect){
	int nbReloc = get_number_reloc_sect(header, sect);
	for(int i=0; i<nbReloc; i++){
		print_sectiondata(file, tab[i].name, tab[i].donnee,header,sect);
	}
}

void free_sect_mod(Table_sect_data* tab, Elf32Hdr header,Tab_Sec* sect){
	int nbReloc = get_number_reloc_sect(header, sect);
	for(int i=0; i<nbReloc; i++){
		free(tab[i].donnee);
		free(tab[i].name);
	}
	free(tab);
}