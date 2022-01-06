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

Elf32Hdr read_elf_header(FILE *file);
int is_big_endian(Elf32Hdr header);
int reverse_endianess(int value,Elf32Hdr header, int half );
void print_ELF_header(Elf32Hdr header);