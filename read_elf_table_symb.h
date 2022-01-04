#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4

#define SHT_SYMTAB 2
#define SHT_STRTAB 3

#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_HIPROC 0xff1f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_HIRESERVE 0xffff

#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3

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
  ELF32_Half     e_shstrndx;          
} Elf32_Ehdr;



typedef struct {
ELF32_Word st_name;
ELF32_Addr st_value;
ELF32_Word st_size;
unsigned char st_info;
unsigned char st_other;
ELF32_Half st_shndx;
} Elf32_Sym;