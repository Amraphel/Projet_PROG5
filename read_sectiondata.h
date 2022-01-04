#include "read_elfSection.h"

char * getSectionName(Elf32Hdr header, FILE* elfFile, int i);

int read_elf_section_data(char * a,Elf32Hdr header, Elf32_Shdr * sections, FILE* file);