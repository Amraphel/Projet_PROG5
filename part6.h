#include "read_relocation_table.h"

int* init_tab_renum(Elf32Hdr header);

Tab_Sec* renumerotation_table_section(Elf32Hdr * header, Tab_Sec* sect, int * tab_renum, int addrText, int addrData);

void renumerotation_table_symbole(FILE* file,Tab_Sym* tab_sym ,Elf32Hdr header,Tab_Sec* tab_sec, int* tab_renum);

void trier_table_symbole(Tab_Sym* tab_sym,Elf32Hdr header,Tab_Sec* tab_sec,FILE* file);