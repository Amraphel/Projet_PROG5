#ifndef ELFSECTION_H 
#define ELFSECTION_H 
#include "read_elfSection.h"

#endif

uint8_t * read_elf_section_data(char * a,Elf32Hdr header, Elf32_Shdr * sections, FILE* file);
void print_sectiondata(FILE* file,char * a, uint8_t * tab, Elf32Hdr header, Elf32_Shdr* sections, int taille, int test, int i);