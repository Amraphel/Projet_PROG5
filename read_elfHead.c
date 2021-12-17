#include "read_elfHead.h"
int reverse_endianess(int value)
{
    int resultat = 0;
    char *source, *destination;
    int i;

    source = (char *)&value;
    destination = ((char *)&resultat) + sizeof(int);
    for (i = 0; i < sizeof(int); i++)
        *(--destination) = *(source++);
    return resultat;
}

int read_elf_header(FILE *elfFile)
{
    Elf32Hdr header;
    fread(&header, 1, sizeof(header), file);
    if (header.e_ident[0] == 0x7f &&
        header.e_ident[1] == 'E' &&
        header.e_ident[2] == 'L' &&
        header.e_ident[3] == 'F')
    {
        printf("En-tête ELF:\n");
        //Affichage de e_ident
        printf("  Magique: ");
        for (int i = 0; i < 16; i++)
        {
            printf(" %02x", header.e_ident[i]);
        }
        printf("\n");

        //Affichage de EI_CLASS
        printf("  Classe:\t");
        switch (header.e_ident[4])
        {
        case 0:
            printf("NONE\n");
            break;
        case 1:
            printf("ELF32\n");
            break;
        case 2:
            printf("ELF64\n");
            break;
        default:
            break;
        }

        //Affichage de EI_DATA
        printf("  Données:\t");
        switch (header.e_ident[5])
        {
        case 0:
            printf("NONE\n");
            break;
        case 1:
            printf("complément à 2, système à octets de poids faible d'abord (little endian)\n");
            break;
        case 2:
            printf("complément à 2, système à octets de poids fort d'abord (big endian)\n");
            break;
        default:
            break;
        }

        //Affichage de EI_VERSION
        printf("  Version:\t");
        switch (header.e_ident[6])
        {
        case 0:
            printf("0 (none)\n");
            break;

        case 1:
            printf("1 (current)\n");
            break;

        default:
            break;
        }

        //Affichage de EI_OS
        printf("  OS/ABI:\t");
        switch (header.e_ident[7])
        {
        case 0:
            printf("UNIX - System V\n");
            break;
        case 1:
            printf("HP-UX\n");
            break;
        case 2:
            printf("NetBSD\n");
            break;
        case 3:
            printf("Linux\n");
            break;
        case 6:
            printf("Sun Solaris\n");
            break;
        case 7:
            printf("IBM AIX\n");
            break;
        case 8:
            printf("SGI Irix\n");
            break;
        case 9:
            printf("FreeBSD\n");
            break;
        case 10:
            printf("Compaq TRU64\n");
            break;
        case 11:
            printf("Novell Modesto\n");
            break;
        case 12:
            printf("OpenBSD\n");
            break;
        case 64:
            printf("ARM EABI\n");
            break;
        case 97:
            printf("ARM\n");
            break;
        case 255:
            printf("Standalone\n");
            break;
        default:
            break;
        }

        //Affichage EI_verABI
        printf("  Version ABI:\t%d\n", header.e_ident[8]);

        //Affichage de e_type
        printf("  Type:\t");

        //printf("%08x\n",reverse_endianess(header.e_type)>>16 );
        switch (reverse_endianess(header.e_type) >> 16)
        {
        case 0x0:
            printf("NONE (pas de type)\n");
            break;
        case 0x1:
            printf("REL (fichier )\n");
            break;
        case 0x2:
            printf("EXEC (fichier exécutable)\n");
            break;
        case 0x3:
            printf("DYN (fichier partagé objet\n");
            break;
        case 0x4:
            printf("CORE (fichier coeur)\n");
            break;
        case 0xff00:
            printf("LOPROC ()\n");
            break;
        case 0xffff:
            printf("HIPROC ()\n");
            break;
        default:
            break;
        }

        //Affichage de e_machine
        printf("  Machine: \t ARM\n");

        //Affichage de e_version
        printf("  Version:\t");

        //printf("%08x\n",header.e_version );
        switch (reverse_endianess(header.e_version))
        {
        case (0x0):
            printf("0x0\n");
            break;
        case (0x1):
            printf("0x1\n");
            break;
        default:
            break;
        }

        //Affichage de e_entry
        printf("  Adresse du point d'entrée:\t 0x%02x \n", reverse_endianess(header.e_entry));

        //Affichage de e_phoff
        printf("  Début des en-têtes de programme: \t %d (octets dans le fichier)\n", reverse_endianess(header.e_phoff));

        //Affichage de e_shoff
        printf("  Début des en-têtes de section: \t %d (octets dans le fichier)\n", reverse_endianess(header.e_shoff));

        //Affichage de e_flags
        printf("  Fanions: \t0x%02x\n", reverse_endianess(header.e_flags));

        //Affichage de e_ehsize
        printf("  Taille de cet en-tête:\t %d (octets)\n", reverse_endianess(header.e_ehsize) >> 16);

        //Affichage de e_phentsize
        printf("  Taille de l'en-tête du programme: :\t %d (octets)\n", reverse_endianess(header.e_phentsize) >> 16);

        //Affichage de e_phnum
        printf("  Nombre d'en-tête du programme:\t %d\n", reverse_endianess(header.e_phnum) >> 16);

        //Affichage de e_shentsize
        printf("  Taille des en-têtes de section:\t %d (octets)\n", reverse_endianess(header.e_shentsize) >> 16);

        //Affichage de e_shnum
        printf("  Nombre d'en-têtes de section:\t %d\n", reverse_endianess(header.e_shnum) >> 16);

        //Affichage de e_shstrndx
        printf("  Table d'indexes des chaînes d'en-tête de section:\t %d\n", reverse_endianess(header.e_shstrndx) >> 16);
    }
    else
    {
        return 1;
        //error
    }

    return 0;
}

int main(int argc, char *argv[])
{
    char *elfile = argv[1];
    FILE *file = fopen(elfFile, "rb");
    if(!file){
        printf("erreur de lecture");
    } else {
            read_elf_header(file);
    }

    fclose(file);
    return 0;
}