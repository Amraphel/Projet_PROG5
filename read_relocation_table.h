#include "read_elfSymbol.h"
#include "read_sectiondata.h"

#define ELF32_R_SYM(i) ((i)>>8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

/* Partie 5*/

typedef struct {
        ELF32_Addr      r_offset;
        ELF32_Word      r_info;
} Elf32_Rel;
 
typedef struct {
        ELF32_Addr      r_offset;
        ELF32_Word      r_info;
        ELF32_Sword     r_addend;
} Elf32_Rela;

typedef struct {
        int indice;
        int taille;
        Elf32_Rel* list_rel;
} Tab_Rel;

/*Fonction de récupération du nombre de section de réadressage
Arguments:
        -header : Le header du fichier
        -sections : Le tableau contenant les en-têtes et noms des sections du fichier
Revoie :
        Le nombre de section de réadressage
*/
int get_number_reloc_sect(Elf32Hdr header, Tab_Sec * sections);

/*Fonction de récupération des données des fonction de réadressage
Arguments:
        -file : l'adresse de lecture du fichier
        -header : Le header du fichier
        -sections : Le tableau contenant les en-têtes et noms des sections du fichier
Renvoie:
        Un tableau contenant la liste des sections de réadressage, avec leur indice,
        leur taille et la liste des réadressages qu'elles contiennent
*/
Tab_Rel* read_temp_reloc_table (FILE* file,Elf32Hdr header, Tab_Sec* sections);


/*Fonction d'affichage de la table de réadressage
Arguments:
        -tab : Le tableau des sections de réadressage avec leur données
        -header : Le header du fichier
        -sections : Le tableau contenant les en-têtes et noms des sections du fichier
        -file : l'adresse de lecture du fichier
        -sym : La table des symboles avec leurs données et leur nom
Effet de bord: 
        Affiche la table des réadressage
*/
void print_reloc_table(Tab_Rel* tab, Elf32Hdr header, Tab_Sec* sections, FILE * file, Tab_Sym* sym);
