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
  ELF32_Half     e_type;              /* Object file type */
  ELF32_Half     e_machine;           /* Architecture */
  ELF32_Word     e_version;           /* Object file version */
  ELF32_Addr     e_entry;             /* Entry point virtual address */
  ELF32_Off      e_phoff;             /* Program header table file offset */
  ELF32_Off      e_shoff;             /* Section header table file offset */
  ELF32_Word     e_flags;             /* Processor-specific flags */
  ELF32_Half     e_ehsize;            /* ELF header size in bytes */
  ELF32_Half     e_phentsize;         /* Program header table entry size */
  ELF32_Half     e_phnum;             /* Program header table entry count */
  ELF32_Half     e_shentsize;         /* Section header table entry size */
  ELF32_Half     e_shnum;             /* Section header table entry count */
  ELF32_Half     e_shstrndx;          /* Section header string table index */
} Elf32Hdr; 

Elf32Hdr read_elf_header(FILE *file);
int reverse_endianess(int value);
int is_little_endian();
