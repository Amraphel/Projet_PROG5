#include "read_elfSymbol.h"
#include "read_sectiondata.h"

#define ELF32_R_SYM(i) ((i)>>8)
#define ELF32_R_TYPE(i) ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

typedef struct {
        ELF32_Addr      r_offset;
        ELF32_Word      r_info;
} Elf32_Rel;
 
typedef struct {
        ELF32_Addr      r_offset;
        ELF32_Word      r_info;
        ELF32_Sword     r_addend;
} Elf32_Rela;

void read_temp_reloc_table (FILE* file,Elf32Hdr header, Elf32_Shdr* sections, Elf32_Sym* sym);
// Elf32_Rel** read_reloc_table(FILE* file,Elf32Hdr header, Elf32_Shdr* sections);
// void print_sect_rel(FILE* file, Elf32Hdr header, Elf32_Shdr* sections,Elf32_Rel** tab, int nbsection, int* tab_entr);
// void liberer_sect_rel(Elf32_Rel** tab, int nbsection);