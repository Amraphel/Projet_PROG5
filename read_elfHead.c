#include "read_elfHead.h"
int is_big_endian(Elf32Hdr header) {
    return header.e_ident[5]==2;
    
}

int reverse_endianess(int value,Elf32Hdr header, int half )
{
    if(is_big_endian(header)){
        int resultat = 0;
        char *source, *destination;
        int i;

        source = (char *)&value;
        destination = ((char *)&resultat) + sizeof(int);
        for (i = 0; i < sizeof(int); i++)
            *(--destination) = *(source++);
        if(half==1){
            return resultat>>16;
        } else {
            return resultat;
        }

    } else {
        return value;
    }
    
}

Elf32Hdr read_elf_header(FILE *file)
{
    Elf32Hdr header;
    fread(&header, 1, sizeof(header), file);
    
    return header;
}

int verify_ELF(Elf32Hdr header){
    return (header.e_ident[0] == 0x7f &&
        header.e_ident[1] == 'E' &&
        header.e_ident[2] == 'L' &&
        header.e_ident[3] == 'F');
}

char * getHclasse(Elf32Hdr header){
    switch (header.e_ident[4])
        {
        case 0:
            return("NONE");
            break;
        case 1:
            return("ELF32");
            break;
        case 2:
            return("ELF64");
            break;
        default:
            return("\n");
            break;
        }
}

char * getHdata(Elf32Hdr header){
    switch (header.e_ident[5])
        {
        case 0:
            return("NONE");
            break;
        case 1:
            return("complément à 2, système à octets de poids faible d'abord (big endian)");
            break;
        case 2:
            return("complément à 2, système à octets de poids fort d'abord (big endian)");
            break;
        default:
            return("\n");
            break;
        }
}

char * getHver(Elf32Hdr header){
    switch (header.e_ident[6])
        {
        case 0:
            return("0 (none)");
            break;

        case 1:
            return("1 (current)");
            break;

        default:
            return("\n");
            break;
        }
}

char * getHos(Elf32Hdr header){
    switch (header.e_ident[7])
        {
        case 0:
            return("UNIX - System V");
            break;
        case 1:
            return("HP-UX");
            break;
        case 2:
            return("NetBSD");
            break;
        case 3:
            return("Linux");
            break;
        case 6:
            return("Sun Solaris");
            break;
        case 7:
            return("IBM AIX");
            break;
        case 8:
            return("SGI Irix");
            break;
        case 9:
            return("FreeBSD");
            break;
        case 10:
            return("Compaq TRU64");
            break;
        case 11:
            return("Novell Modesto");
            break;
        case 12:
            return("OpenBSD");
            break;
        case 64:
            return("ARM EABI");
            break;
        case 97:
            return("ARM");
            break;
        case 255:
            return("Standalone");
            break;
        default:
            return("\n");
            break;
        }
}

char * getHtype(Elf32Hdr header){
    switch (reverse_endianess(header.e_type, header, 1))
    {
    case 0x0:
        return("NONE (pas de type)");
        break;
    case 0x1:
        return("REL (fichier de réadressage)");
        break;
    case 0x2:
        return("EXEC (fichier exécutable)");
        break;
    case 0x3:
        return("DYN (fichier partagé objet)");
        break;
    case 0x4:
        return("CORE (fichier coeur)");
        break;
    case 0xff00:
        return("LOPROC ()");
        break;
    case 0xffff:
        return("HIPROC ()");
        break;
    default:
        return("\n");
        break;
    }
}

void print_ELF_header(Elf32Hdr header){
    if (verify_ELF(header))
    {
        printf("%-33s\n","En-tête ELF :");
        //Affichage de e_ident
        printf("  %s\t", "Magique:");
        for (int i = 0; i < 16; i++)
        {
            printf(" %02x", header.e_ident[i]);
        }
        printf("\n");

        //Affichage de EI_CLASS
        printf("  %-35s\t","Classe:");
        printf("%s\n", getHclasse(header));

        //Affichage de EI_DATA
        printf("  %-35s\t","Données:");
        printf("%s\n", getHdata(header));

        //Affichage de EI_VERSION
        printf("  %-35s\t","Version:");
        printf("%s\n", getHver(header));

        //Affichage de EI_OS
        printf("  %-35s\t","OS/ABI:");
        printf("%s", getHos(header));
        printf("\n");

        //Affichage EI_verABI
        printf("  %-35s\t%d\n","Version ABI:", header.e_ident[8]);

        //Affichage de e_type
        printf("  %-35s\t","Type:");
        printf("%s\n", getHtype(header));


        //Affichage de e_machine
        if(reverse_endianess(header.e_machine, header, 1)==40){
            printf("  %-35s\tARM\n","Machine:");
        } else {
            printf("  %-35s\tNon-ARM\n","Machine:");
        }


        //Affichage de e_version
        printf("  %-35s\t","Version:");

        switch (reverse_endianess(header.e_version, header, 0))
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
        printf("  %-35s\t 0x%02x \n","Adresse du point d'entrée:" ,reverse_endianess(header.e_entry, header,0));

        //Affichage de e_phoff
        // printf("%08x\n",header.e_phoff) ;
        printf("  %-35s\t %d (octets dans le fichier)\n","Début des en-têtes de programme:", reverse_endianess(header.e_phoff, header,0));

        //Affichage de e_shoff
        printf("  %-35s\t %d (octets dans le fichier)\n","Début des en-têtes de section:", reverse_endianess(header.e_shoff, header,0));

        //Affichage de e_flags
        printf("  %-35s\t 0x%x\n","Fanions:", reverse_endianess(header.e_flags,header,0));

        //Affichage de e_ehsize
        printf("  %-35s\t %d (octets)\n","Taille de cet en-tête:", reverse_endianess(header.e_ehsize,header,1));

        //Affichage de e_phentsize
        printf("  %-35s\t %d (octets)\n","Taille de l'en-tête du programme:" ,reverse_endianess(header.e_phentsize,header,1));

        //Affichage de e_phnum
        printf("  %-35s\t %d\n","Nombre d'en-tête du programme:", reverse_endianess(header.e_phnum, header,1));

        //Affichage de e_shentsize
        printf("  %-35s\t %d (octets)\n","Taille des en-têtes de section:", reverse_endianess(header.e_shentsize,header,1));

        //Affichage de e_shnum
        printf("  %-35s\t %d\n","Nombre d'en-têtes de section:", reverse_endianess(header.e_shnum,header,1));

        //Affichage de e_shstrndx
        printf("  Table d'indexes des chaînes d'en-tête de section:\t %d\n", reverse_endianess(header.e_shstrndx,header,1));
    }
    else
    {
        fprintf(stderr,"Fichier pas ELF");
        exit(1);
    }

}

// int main(int argc, char *argv[])
// {
//     char *elfile = argv[1];
//     FILE *file = fopen(elfile, "rb");
//     if(!file){
//         printf("erreur de lecture");
//     } else {
//             Elf32Hdr header= read_elf_header(file);
//             print_ELF_header(header);
//     }

//     fclose(file);
//     return 0;
// }