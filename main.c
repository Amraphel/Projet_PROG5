#include "part6.h"

int main(int argc, char *argv[]){
	if (argc==1){
		printf("Pas le bon nombre d'arguments : il faut au moins donner un nom de fichier."); }
	else{
		// On commence par vérifier que le second paramètre est bien une option
		if (argv[1][0] != '-'){ // si ce n'est pas une option, on affiche la liste des options
			printf("Option nécessaire pour utiliser la fonction readelf. Liste des options disponibles :\n");
			printf(" -h   pour afficher le header\n");
			printf(" -S   pour afficher la table des sections\n");
			printf(" -x nb   pour afficher le contenu de la section nb\n");
			printf(" -s   pour afficher la table des symboles\n");
			printf(" -r   pour afficher la tale des réimplantations\n"); 
			printf(" -a   pour tout afficher (hormis le contenu de chacune des sections)\n ");}
		else {
  		  	FILE *file = fopen(argv[argc-1], "rb"); 
   		 	if(!file){
   	    		printf("Erreur de lecture !\n"); }
			else {
				// on initialise des variables booléennes pour chacune des options disponibles
				int option_h = 0;
				int option_S = 0;
				int option_x = 0;
				char * valeur_x = 0;
				int option_s = 0;
				int option_r = 0;
				int option_err = 0;
				int i = 1;	

				// boucle pour déterminer les options qui sont demandées
				// on s'arrete au paramètre avant le fichier
				while (i<argc-1){
					if (argv[i][1] == 'h'){
						option_h = 1; }
					else if (argv[i][1] == 'S'){
						option_S = 1; }
					else if (argv[i][1] == 'x'){
						option_x = 1; 
						i++;
						valeur_x = argv[i];}
					else if (argv[i][1] == 's'){
						option_s = 1; }
					else if (argv[i][1] == 'r'){
						option_r = 1; }
					else if (argv[i][1] == 'a'){
						option_h = 1;
						option_S = 1;
						option_s = 1;
						option_r = 1; }
					else {
						option_err = 1; }
					i++; }

				// on appelle les fonctions de lecture
	        	Tab_Sec * sect;
				uint8_t *tab;
				Elf32Hdr header = read_elf_header(file); //lecture partie 1
   		     	sect = read_elf_section(file, header); // lecture partie 2
				if (option_x == 1){ // on ne lit dans la partie 3 que si l'option est activée
					tab = read_elf_section_data(valeur_x,header,sect,file); } 
				Tab_Sym * sym = renvoyer_table_sym(file,header,sect); //lecture partie 4
				Tab_Rel* tab_reloc= read_temp_reloc_table(file,header,sect);

				// on affiche seulement les parties dont les options ont été mises
				if (option_h == 1){
					print_ELF_header(header); }
				if (option_S == 1){
					print_elf_section(header,sect,file);
					int* tab_renum= init_tab_renum(header);
					Tab_Sec* sect2= renumerotation_table_section(&header, sect, tab_renum);
					print_elf_section(header,sect2,file);
					free(sect2);
					free(tab_renum); 
					}
				if (option_x == 1){
					print_sectiondata(file,valeur_x,tab,header,sect);
					free(tab); }
				if (option_s == 1){
					// int* tab_renum= init_tab_renum(header);
					// Elf32_Shdr* sect2= renumerotation_table_section(header, sect, tab_renum);
					affiche_table_Symboles(file,sect,header,sym);
					// renumerotation_table_symbole(file, sym, header, sect2, tab_renum);
					// affiche_table_Symboles(file,sect,header,sym);
					// free(sect2);
					// free(tab_renum);
					 }
				if (option_r == 1){
					print_reloc_table(tab_reloc,header,sect,file,sym); }
				if (option_err == 1){
					printf("Une des options choisies est incorrecte.\n"); }
				free(tab_reloc);
				free(sect);
				free(sym);
				}
	    	fclose(file);
			}
		}

    return 0;
}
