#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t ELF32_Addr ;
typedef uint16_t ELF32_Half ;
typedef uint32_t ELF32_Off ;
typedef int32_t ELF32_Sword ;
typedef uint32_t ELF32_Word ;


typedef struct {
  unsigned char  e_ident[16];         /* Magic number and other info */
  ELF32_Half     e_type;             
  ELF32_Half     e_machine;         
  ELF32_Word     e_version;          
  ELF32_Addr     e_entry;        
  ELF32_Off      e_phoff;       
  ELF32_Off      e_shoff;           
  ELF32_Word     e_flags;            
  ELF32_Half     e_ehsize;        
  ELF32_Half     e_phentsize;         
  ELF32_Half     e_phnum;            
  ELF32_Half     e_shentsize;         
  ELF32_Half     e_shnum;             
  ELF32_Half     e_shstrndx;          
} Elf32Hdr; 

int is_little_endian();

/*Fonction de test du big endian
Arguments:
  -header : Le header du fichier
Renvoie:
  0 si le fichier est en little endian, 1 s'il est en big endian
*/
int is_big_endian(Elf32Hdr header);

/*Fonction d'inversion des bit de poids fort et faible
Arguments:
  -value : La valeur à modifier
  -header : Le header du fichier
  -half : Un indicateur pour forcer un décalage de 16 bits sur la droite si 1, 
          ou ne pas le faire si 0
Renvoie:
  Soit la valeur originale si elle était déjà en little endian
  Soit la valeur modifié en little endian
  Soit la valuer modifié en little endian avec un décalage de 16bits sur la droite
*/
int reverse_endianess(int value,Elf32Hdr header, int half );

/*Fonction de récupération du header
Arguments:
  -file : L'addresse de lecture du fichier 
Renvoie:
  Le header du fichier
*/
Elf32Hdr read_elf_header(FILE *file);
/*Fonction de vérification du type du fichier
Arguments:
  -header : Le header du fichier
Renvoie:
  1 si le fichier est de type ELF, 0 sinon
*/
int verify_ELF(Elf32Hdr header);

/*Fonction de récupération de la classe du fichier
Arguments:
  -header : Le header du fichier
Renvoie:
  Un char* correspondant à la classe du fichier
*/
char * getHclasse(Elf32Hdr header);

/*Fonction de récupération type de données du fichier
Arguments:
  -header : Le header du fichier
Renvoie:
  Un char* correspondant type de données du fichier
*/
char * getHdata(Elf32Hdr header);

/*Fonction de récupération de la version du fichier
Arguments:
  -header : Le header du fichier
Renvoie:
  Un char* correspondant à la version du fichier
*/
char * getHver(Elf32Hdr header);

/*Fonction de récupération de l'OS du fichier
Arguments:
  -header : Le header du fichier
Renvoie:
  Un char* correspondant à l'OS du fichier
*/
char * getHos(Elf32Hdr header);

/*Fonction de récupération du type du fichier
Arguments:
  -header : Le header du fichier
Renvoie:
  Un char* correspondant au type du fichier
*/
char * getHtype(Elf32Hdr header);

/*Fonction d'affichage du header du fichier
Arguments:
  -header : Le header du fichier
*/
void print_ELF_header(Elf32Hdr header);