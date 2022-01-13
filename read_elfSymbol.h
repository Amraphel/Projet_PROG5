#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ELFSECTION_H 
#define ELFSECTION_H 
#include "read_elfSection.h"
#endif
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4

#define SHT_SYMTAB 2
#define SHT_STRTAB 3

#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_HIPROC 0xff1f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_HIRESERVE 0xffff

#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3

/* Partie 4*/

typedef uint32_t ELF32_Addr ;
typedef uint16_t ELF32_Half ;
typedef uint32_t ELF32_Off ;
typedef int32_t ELF32_Sword ;
typedef uint32_t ELF32_Word ;

typedef struct {
    ELF32_Word st_name;
    ELF32_Addr st_value;
    ELF32_Word st_size;
    unsigned char st_info;
    unsigned char st_other;
    ELF32_Half st_shndx;
} Elf32_Sym;

typedef struct {
    Elf32_Sym symbole;
    unsigned char * name;
} Tab_Sym;

/* Fonction de lecture de symbole
Arguments:
    -file : L'adresse de lecture du fichier
    -header : Le header du fichier
Renvoie;
    Le symbole correspondante à l'emplacement de l'adresse de lecture
*/
Elf32_Sym lire_un_symbole(FILE * file, Elf32Hdr header);

/* Fonction de recupération de la taille de la table des symbole
Arguments:
    -file : L'adresse de lecture du fichier
    -header : Le header du fichier
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
Renvoie;
    La taille de la table des symboles
*/
int get_taille_table_symbole (FILE * file, Tab_Sec* tab_sec, Elf32Hdr header);

/*Fonction de lecture de la table des symboles
Arguments:
    -file : L'adresse de lecture du fichier 
    -header : Le header du fichier
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
Renvoie:
    Un pointeur sur un tableau contenant les données et les noms des différents symboles
    contenus dans la tables des symboles
*/
Tab_Sym*  renvoyer_table_sym(FILE * file, Elf32Hdr header , Tab_Sec* tab_sec);

/*Fonction récupération du nom d'un symbole
Arguments:
    -indice : L'indice du symbole dont on affiche le nom
    -file : L'adresse de lecture du fichier 
    -header : Le header du fichier
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
Renvoie:
    Le nom du symbole correspondant à l'indice souhaité
*/
unsigned char * renvoyer_nom_du_symbole(int indice, FILE * file,Elf32Hdr header,Tab_Sec* tab_sec);

/*Fonction d'affichage de la table des symboles
Arguments:
    -file : L'adresse de lecture du fichier 
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
    -header : Le header du fichier
Effet de bord:
    Affiche la table des symboles
*/
void affiche_table_Symboles(FILE *file,Tab_Sec* tab_sec,Elf32Hdr header, Tab_Sym* tab_sym);