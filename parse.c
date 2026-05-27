#include "cink.h"

bool validate(const Object *obj)
{
    if (obj->size < sizeof(ELFHeader)) {
        log(CORVUS_ERROR, "not an elf file: too small");
        return false;
    }
    if (memcmp(obj->content, ELFMAG, strlen(ELFMAG)) != 0) {
        log(CORVUS_ERROR, "not an elf file: magic number");
        return false;
    }
    return true;
}

void parse_elf_header(Object *obj)
{
    obj->ehdr = *(ELFHeader *) obj->content;
}

void parse_section(Object *obj)
{
    const char *shoff = obj->content + obj->ehdr.e_shoff;
    int nshdrs = obj->ehdr.e_shnum;

    /* collect null section header */
    ELFSectionHeader null_shdr = *(ELFSectionHeader *) shoff;
    shoff += sizeof(ELFSectionHeader);
    arrpush(obj->shdrs, null_shdr);

    /* NOTE: If e_shnum == 0, the actual section count is stored
       in the sh_size field of the null section header (section 0) */
    if (nshdrs == 0) nshdrs = null_shdr.sh_size;

    /* collect section header */
    for (int i = 1; i < nshdrs; i++) {
        ELFSectionHeader shdr = *(ELFSectionHeader *) shoff;
        arrpush(obj->shdrs, shdr);
        shoff += sizeof(ELFSectionHeader);
    }

    /* NOTE: If e_shstrndex == SHN_XINDEX, the actual section header string table index
       is stored in the sh_link field of the null section header (section 0) */
    int shstridx = obj->ehdr.e_shstrndx == SHN_XINDEX ? null_shdr.sh_link : obj->ehdr.e_shstrndx;
    obj->shstroff = obj->content + obj->shdrs[shstridx].sh_offset;
}

void parse_symbol(Object *obj)
{
    /* collect symbol */
    const ELFSectionHeader *symsechdr = object_get_shdr(obj, SHT_SYMTAB);
    assert(symsechdr);
    const char *symoff = obj->content + symsechdr->sh_offset;
    obj->symstroff = obj->content + obj->shdrs[symsechdr->sh_link].sh_offset;
    int nsyms = symsechdr->sh_size / sizeof(ELFSymbol);
    for (int i = 0; i < nsyms; i++) {
        ELFSymbol sym = *(ELFSymbol *) symoff;
        arrpush(obj->syms, sym);
        symoff += sizeof(ELFSymbol);
    }
}
