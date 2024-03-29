#include "read_relocation_table.h"
/*Partie 6*/

/*Fonction de création de la table de renumérotation
Arguments:
    -header : Le header du fichier
Renvoie:
    Un tableau contenant les nouveaux indices de sections en fonction des anciens
*/
int* init_tab_renum(Elf32Hdr header);

/*Fonction de comptage du nombre de section vide
Arguments:
    -header : Le header du fichier
    -sect : Le tableau contenant les en-têtes et noms des sections du fichier
Renvoie:
    Le nombre de section vide
*/
int nbSectVide(Elf32Hdr * header, Tab_Sec* sect);


/*Fonction de renumérotation des sections
Arguments:
    -header : Le header du fichier
    -sect : Le tableau contenant les en-têtes et noms des sections du fichier
    -tab_renum : La table des renumérotations
    -addrText: contient l'adresse de la section .text à appliquer
    -addrData: contient l'adresse de la section .data à appliquer
Renvoie:
    La nouvelle table des sections renumérotée avec les changement de décalage pour chaque section
*/
Tab_Sec* renumerotation_table_section(Elf32Hdr * header, Tab_Sec* sect, int * tab_renum, int addrText, int addrData);
   
/*------------------------------------------------------------------------------------------------------------------------------*/
/*Partie 7*/

/*Fonction de renumérotation et correction de la table des symbole
Arguments:
    -file : L'adresse de lecture du fichier
    -tab_sym : La table des symboles avec leurs données et leur nom
    -header : Le header du fichier
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
    -tab_renum : La table des renumérotations
Effet de bord:
    Modifie la table des symboles avec les nouvelles valeurs
*/
void renumerotation_table_symbole(FILE* file,Tab_Sym* tab_sym ,Elf32Hdr header,Tab_Sec* tab_sec, int* tab_renum);

/*Fonction de trie de la table des symboles
Arguments:
    -tab_sym : La table des symboles avec leurs données et leur nom
    -header : Le header du fichier
    -tab_sec : Le tableau contenant les en-têtes et noms des sections du fichier
    -file : L'adresse de lecture du fichier
Effet de bord:
    Trie la table des symboles en fonction de leur type (non utilisé pour l'instant)
*/
void trier_table_symbole(Tab_Sym* tab_sym,Elf32Hdr header,Tab_Sec* tab_sec,FILE* file);