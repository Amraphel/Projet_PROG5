#include "read_relocation_table.h"

int taille_sect_data;
int test_sect_data;
int indice_sect_data;

int main(int argc, char *argv[]){
    FILE *file = fopen(argv[1], "rb");
    if(!file){
        printf("erreur de lecture");
    } else {
		//on commence d'abord par appeler toutes les fonctions d'écriture
        Elf32_Shdr * sect;
		Elf32Hdr header = read_elf_header(file); //lecture partie 1
        sect = read_elf_section(file, header); // lecture partie 2
        Elf32_Sym * sym = renvoyer_table_sym(file,header,sect);
		//fonction lecture partie 5 (quand elle sera finie)

		//on considère que s'il n'y a pas d'options ajoutées, on affiche tout
		if (argc==2){ 
			print_ELF_header(header); //part 1
			print_elf_section(header,sect,file);//part 2
			// pas de part 3 car nécessite une option
            read_temp_reloc_table(file,header,sect,sym);
			affiche_table_Symboles(file, sect ,header);//part 4
			// print_reloc_table (fonction afficahge partie 5)
		}

		//cas de la partie 3 (quand on a une lettre et un autre paramètre)
		else if (argc==4){ 
			if (*argv[2] == 'x'){
                uint8_t* tab;
				tab=read_elf_section_data(argv[3],header,sect,file);
                print_sectiondata(file,argv[3],tab,header,sect,taille_sect_data,test_sect_data,indice_sect_data);
                free(tab);}
			else {
				printf("Paramètres entrés non valides."); }
		}

		//cas quand on a une option			
		else if (argc == 3){
			switch(*argv[2]){
				case 'a': 	// on affiche tout
						print_ELF_header(header); //part 1
						print_elf_section(header,sect,file);//part 2
                        read_temp_reloc_table(file, header, sect, sym);
						affiche_table_Symboles(file, sect, header);//part 4
						// print_reloc_table (fonction afficahge partie 5)
						break;
				case 'h': // on affiche que la partie 1
						print_ELF_header(header);
						break;
				case 'S': // on affiche que la partie 2
						print_elf_section(header,sect,file);
						break;
				case 's': // on affiche que la partie 4
						affiche_table_Symboles(file, sect, header);
						break;
				case 'r': // on affiche que la partie 5
						read_temp_reloc_table(file,header,sect,sym);
						break;
				default : break;} }

		else {
			printf("Pas le bon nombre d'arguments.\n");
			printf(" Le format est le suivant : ./programme nom_fichier option_1 option_2\n"); 
            }
            free(sect);
		}

    fclose(file);
    return 0;
}
