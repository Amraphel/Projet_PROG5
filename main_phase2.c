				#include "reimplantation_ARM.h"

int main(int argc, char *argv[]){
	if (argc==1){
		printf("Pas le bon nombre d'arguments : il faut au moins donner un nom de fichier."); }
	else{
		// On commence par vérifier que le second paramètre est bien une option
		if (argv[1][0] != '-'){ // si ce n'est pas une option, on affiche la liste des options
			printf("Option nécessaire pour utiliser la fonction readelf. Liste des options disponibles :\n");
			printf(" -h   pour afficher le header\n");
			printf(" -S   pour afficher la table des sections\n");
			printf(" -m   pour afficher le contenu des sections modifiées\n");
			printf(" -s   pour afficher la table des symboles\n");
			printf(" -r   pour afficher la tale des réadressages\n"); 
			printf(" -a   pour tout afficher (hormis le contenu de chacune des sections)\n ");}
		else {
  		  	FILE *file = fopen(argv[2], "rb"); 
   		 	if(!file){
   	    		printf("Erreur de lecture !\n"); }
			else {
				// on initialise des variables booléennes pour chacune des options disponibles
				int option_h = 0;
				int option_S = 0;
				int option_m = 0;
				int option_s = 0;
				int option_r = 0;
				int option_err = 0;
				int i = 1;	

				// boucle pour déterminer les options qui sont demandées
				// on s'arrete au paramètre avant le fichier
                if(argc<5){
                    printf("Pas assez d'arguments\n");
                    printf("Il faut appliquer le programme sous la forme: -option fichier addr_Text addr_Data\n");
                }else {
                    while (i<argc-3){
					if (argv[i][1] == 'h'){
						option_h = 1; }
					else if (argv[i][1] == 'S'){
						option_S = 1; }
					else if (argv[i][1] == 'm'){
						option_m = 1; }
					else if (argv[i][1] == 's'){
						option_s = 1; }
					else if (argv[i][1] == 'r'){
						option_r = 1; }
					else if (argv[i][1] == 'a'){
						option_h = 1;
						option_S = 1;
						option_s = 1;
						option_r = 1;
                        option_m = 1; }
					else {
						option_err = 1; }
					i++; }

                    // on appelle les fonctions de lecture
                    Tab_Sec * sect;
                    Elf32Hdr header = read_elf_header(file); //lecture partie 1
                    sect = read_elf_section(file, header); // lecture partie 2
                    Tab_Sym * sym = renvoyer_table_sym(file,header,sect); //lecture partie 4
                    Tab_Rel* tab_reloc= read_temp_reloc_table(file,header,sect);
                    int* tab_renum= init_tab_renum(header);
                    Elf32Hdr newheader = header;
                    //.text_addr 32, .data_addr 10240
                    int addr_text=atoi(argv[3]);
                    int addr_data=atoi(argv[4]);
                    Tab_Sec* sect2= renumerotation_table_section(&newheader, sect, tab_renum,addr_text,addr_data);
                    renumerotation_table_symbole(file, sym, newheader, sect2, tab_renum);
                    Table_sect_data * tab_sec_mod_data= write_reimp(file, header, tab_reloc, sect,sect2, sym,tab_renum);
                    // on affiche seulement les parties dont les options ont été mises
                    if (option_h == 1){
                        print_ELF_header(newheader); }
                    if (option_S == 1){
                        print_elf_section(newheader,sect2,file);
                        }
                    if (option_m == 1){
                        print_sect_mod(file, tab_sec_mod_data,header,sect);
                        }
                    if (option_r == 1){
                        print_reloc_table(tab_reloc,header,sect2,file,sym); }
                    if (option_s == 1){
                        affiche_table_Symboles(file,sect,header,sym);
                        }
                    if (option_err == 1){
                        printf("Une des options choisies est incorrecte.\n"); }


                    
                    free_sect_mod(tab_sec_mod_data,header,sect);
                    free(sect2);
                    free(tab_renum);
                    free(tab_reloc);
                    free(sect);
                    free(sym);
                    }
                }
	    	fclose(file);
			}
		}

    return 0;
}

                
                
                