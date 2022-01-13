#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "read_elfHead.h"

typedef uint32_t ELF32_Addr ;
typedef uint16_t ELF32_Half ;
typedef uint32_t ELF32_Off ;
typedef int32_t ELF32_Sword ;
typedef uint32_t ELF32_Word ;

/*Partie 2*/

typedef struct {
    ELF32_Word sh_name;
    ELF32_Word sh_type;
    ELF32_Word sh_flags;
    ELF32_Addr sh_addr;
    ELF32_Off sh_offset;
    ELF32_Word sh_size;
    ELF32_Word sh_link;
    ELF32_Word sh_info;
    ELF32_Word sh_addralign;
    ELF32_Word sh_entsize;
} Elf32_Shdr;

typedef struct{
    Elf32_Shdr section;
    char* name;
} Tab_Sec;

/* Fonction de lecture de section
Arguments:
    -file : L'adresse de lecture du fichier
    -header : Le header du fichier
Renvoie;
    La section correspondante à l'emplacement de l'adresse de lecture
*/
Elf32_Shdr lire_une_section(FILE* file, Elf32Hdr header);

/*Fonction de stockage du tableau des sections
Arguments:
    -elfFile : L'adresse de lecture du fichier 
    -header : Le header du fichier
Renvoie:
    Un tableau contenant les en-têtes et noms sections du fichier
*/
Tab_Sec * read_elf_section(FILE *elfFile, Elf32Hdr header);
/*Fonction d'affichage des sections
Arguments:
    -header : Le header du fichier
    -sections : Le tableau contenant les en-têtes et noms des sections du fichier
    -elfFile : L'adresse de lecture du fichier 
Effet de bord:
    Affiche le tableau des sections
*/
void print_elf_section(Elf32Hdr header, Tab_Sec * sections, FILE* elfFile);

/*Fonction de récupération du nom de la section
Arguments:
    -header : Le header du fichier
    -elfFile : L'adresse de lecture du fichier
    -i : numéro de la section dont on récupère le nom 
Renvoie:
    Le nom de la section
*/
char * getSectionName(Elf32Hdr header, FILE* elfFile, int i);

/*Fonction de récupération du type de la section
Arguments:
    -sh_type : La valeur du type de la section voulue
    -name : Le nom de la section
Renvoie:
    Le type de la section voulue
*/
char * get_section_type(ELF32_Word sh_type, char * name);

/*Fonction de récupération du flag de la section
Arguments:
    -sh_flags : La valeur du flags de la section voulue
    -flag_ind : Un pointeur égale à 0 indiquant le nombre de flags différents
Effet de bord:
    Change la valeur de flag_ind en fonction du nombre de flags
Renvoie:
    Les flags à afficher
*/
char * get_section_flag(ELF32_Word sh_flags, int * flag_ind);