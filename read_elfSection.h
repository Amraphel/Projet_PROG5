#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "read_elfHead.h"

typedef uint32_t ELF32_Addr ;
typedef uint16_t ELF32_Half ;
typedef uint32_t ELF32_Off ;
typedef int32_t ELF32_Sword ;
typedef uint32_t ELF32_Word ;


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

/*Fonction de stockage du tableau des sections
Arguments:
    -elfFile : L'addresse de lecture du fichier 
    -header : Le header du fichier
Renvoie:
    Un tableau contenant les sections du fichier
*/
Elf32_Shdr * read_elf_section(FILE *elfFile, Elf32Hdr header);
/*Fonction d'affichage des sections
Arguments:
    -header : Le header du fichier
    -sections : Le tableau des sections du fichier
    -elfFile : L'addresse de lecture du fichier 
*/
void print_elf_section(Elf32Hdr header, Elf32_Shdr * sections, FILE* elfFile);

/*Fonction de récupération du nom de la section
Arguments:
    -header : Le header du fichier
    -elfFile : L'addresse de lecture du fichier
    -i : numéro de la section dont on récupère le nom 
*/
char * getSectionName(Elf32Hdr header, FILE* elfFile, int i);

/*Fonction de récupération du type de la section

*/
char * get_section_type(ELF32_Word sh_type, char * name);

/*Fonction de récupération du flag de la section

*/
char * get_section_flag(ELF32_Word sh_flags, int * flag_ind);