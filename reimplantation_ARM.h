#include "renum_correction.h"
/*Partie 8*/


void r_arm_jump(FILE* file,uint8_t* tab, Elf32Hdr header, Tab_Rel* tab_reloc, Tab_Sec* tab_sec,Tab_Sec* new_tab_sec,Tab_Sym* sym, int ind_sect, int ind_sect_reloc, int ind_reloc, int* tab_renum);

/*Fonction de renumérotation de la table des symbole
Arguments:
    -file : L'adresse de lecture du fichier
    -tab : Les données de la section lue
    -header : Le header du fichier
    -tab_reloc : La table de réadressage du fichier
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
    -sym : La table des symboles avec leurs données et leur nom
    -ind_sect : L'indice de la section que l'on modifie
    -ind_sect_reloc : L'indice de la section de réadressage concernée
    -ind_reloc : L'indice du réadressage de la section concernée
Effet de bord:
    Modifie les données du tableau tab lors d'un réadressage de type arm_abs32
*/
void r_arm_abs32(FILE* file,uint8_t* tab, Elf32Hdr header, Tab_Rel* tab_reloc, Tab_Sec* tab_sec,Tab_Sym* sym, int ind_sect, int ind_sect_reloc, int ind_reloc);


/*Fonction de renumérotation de la table des symbole
Arguments:
    -tab : Les données de la section lue
    -val: La valeur à remplacer dans le tableau
    -offset : L'indice où l'on doit se place dans le tableau
Effet de bord:
    Modifie les données du tableau tab avec les bonnes valeurs
*/
void rewrite_tab(uint8_t* tab, uint32_t val, uint32_t offset);


/*Fonction de renumérotation de la table des symbole
Arguments:
    -file : L'adresse de lecture du fichier
    -header : Le header du fichier
    -tab_reloc : La table de réadressage du fichier
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
    -sym : La table des symboles avec leurs données et leur nom
Renvoie:
    Un tableau contenant les nouvelles données et le nom des sections modifiées
    par le réadressage    
*/
Table_sect_data * write_reimp(FILE* file, Elf32Hdr header, Tab_Rel* tab_reloc, Tab_Sec* sect,Tab_Sec* new_tab_sec,Tab_Sym* sym, int* tab_renum);

/*Fonction de renumérotation de la table des symbole
Arguments:
    -file : L'adresse de lecture du fichier
    -tab : Le tableau des sections modifiées avec leurs données et leur nom
    -header : Le header du fichier
    -sect : Le tableau contenant les en-têtes et noms des sections du fichier
Effet de bord:
    Affiche les données des différentes sections modifiées
*/
void print_sect_mod(FILE * file, Table_sect_data* tab, Elf32Hdr header, Tab_Sec* sect);

/*Fonction de renumérotation de la table des symbole
Arguments:
    -tab : Le tableau des sections modifiées avec leurs données et leur nom
    -header : Le header du fichier
    -sect : Le tableau contenant les en-têtes et noms des sections du fichier
Effet de bord:
    Libère le tableau de sections modifiées
*/
void free_sect_mod(Table_sect_data* tab, Elf32Hdr header,Tab_Sec* sect);