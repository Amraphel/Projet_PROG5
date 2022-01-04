#include "read_sectiondata.h"


typedef struct {
        ELF32_Word      st_name;
        ELF32_Addr      st_value;
        ELF32_Word      st_size;
        unsigned char   st_info;
        unsigned char   st_other;
        ELF32_Half      st_shndx;
} Elf32_Sym;

typedef struct {
        ELF32_Addr      r_offset;
        ELF32_Word      r_info;
} Elf32_Rel;
 
typedef struct {
        ELF32_Addr      r_offset;
        ELF32_Word      r_info;
        ELF32_Sword     r_addend;
} Elf32_Rela;