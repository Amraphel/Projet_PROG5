#include "read_relocation_table.h"

int* init_tab_renum(Elf32Hdr header);

Elf32_Shdr* renumerotation_table_section(Elf32Hdr header, Elf32_Shdr * sect, int * tab_renum);

void renumerotation_table_symbole(FILE* file,Elf32_Sym* tab_sym ,Elf32Hdr header,Elf32_Shdr* tab_sec, int* tab_renum);

void trier_table_symbole(Elf32_Sym* tab_sym,Elf32Hdr header,Elf32_Shdr* tab_sec,FILE* file);