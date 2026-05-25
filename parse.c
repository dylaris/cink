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
    obj->elf_header = *(ELFHeader *) obj->content;
}

void parse_section_header(Object *obj)
{
    const char *cursor = obj->content + obj->elf_header.e_shoff;
    size_t num_sections = obj->elf_header.e_shnum;

    /* collect null section header */
    ELFSectionHeader null_section_header = *(ELFSectionHeader *) cursor;
    cursor += sizeof(ELFSectionHeader);
    arrpush(obj->sections, (Section) { .header = null_section_header });

    /* NOTE: If e_shnum == SHN_UNDEF (0xffff), the actual section count is stored
       in the sh_link field of the null section header (section 0) */
    if (num_sections == (u16) -1) num_sections = null_section_header.sh_link;

    /* collect section header and section content */
    for (size_t i = 1; i < num_sections; i++) {
        ELFSectionHeader section_header = *(ELFSectionHeader *) cursor;
        cursor += sizeof(ELFSectionHeader);
        Section section = {
            .header = section_header,
            .content = { .data = obj->content + section_header.sh_offset, .len = section_header.sh_size },
        };
        arrpush(obj->sections, section);
    }

    /* collect section name */
    cursor = obj->sections[obj->elf_header.e_shstrndx].content.data; /* section header string table */
    arrforeach(Section, obj->sections) {
        it->name = svfromcstr(cursor + it->header.sh_name);
    }
}

void parse_symbol(Object *obj)
{
    (void) obj;
}
