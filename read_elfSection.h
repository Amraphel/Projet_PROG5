#include <stdint.h>
#include <stdio.h>
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

Elf32_Shdr* read_elf_section(FILE *elfFile, Elf32Hdr header);