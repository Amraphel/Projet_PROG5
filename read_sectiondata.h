#ifndef ELFSECTION_H 
#define ELFSECTION_H 
#include "read_elfSection.h"

#endif


/*Fonction de lecture des données de la section 
Arguments:
    -name : Le nom ou le numéro de la section recherchée
    -header : Le header du fichier
    -sections : Le tableau des sections du fichier
    -file : L'addresse de lecture du fichier 
Renvoie:
    Un tableau d'octets correspondant aux données de la section
*/
uint8_t * read_elf_section_data(char * name,Elf32Hdr header, Tab_Sec * sections, FILE* file);


/*Fonction de verification de la présence d'une section
Arguments:
    -name : Le nom ou le numéro de la section recherchée
    -file : l'addresse de lecture du fichier
    -header : Le header du fichier
Renvoie:
    Un entier égale à -1 si la section de nom ou numéro name existe pas, sinon l'indice de cette section
*/
int verify_sect_data(char * name,FILE* file, Elf32Hdr header);

/*Fonction de vérification de la taille de la section 
Arguments:
    -sections : Le tableau des sections du fichier
    -header : Le header du fichier
    -indice_sect_data : L'indice de la section à vérifier
Renvoie:
    La taille de la section à vérifier
*/
int verify_taille_sect(Tab_Sec* sections, int indice_sect_data, Elf32Hdr header);

/*Fonction d'affichage des données de la section     
    -file : L'addresse de lecture du fichier 
    -name : Le nom de la section à afficher
    -tab : Le tableau des données de la section à afficher
    -header : Le header du fichier
    -sections : Le tableau des sections du fichier
*/
void print_sectiondata(FILE* file,char * name, uint8_t * tab, Elf32Hdr header, Tab_Sec* sections);