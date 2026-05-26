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
    /* collect section header and section content */
    const char *section_header_table_offset = obj->content + obj->elf_header.e_shoff;
    int num_sections = obj->elf_header.e_shnum;
    for (int i = 0; i < num_sections; i++) {
        ELFSectionHeader section_header = *(ELFSectionHeader *) section_header_table_offset;
        Section section = {
            .header = section_header,
            .content = { .data = obj->content + section_header.sh_offset, .len = section_header.sh_size },
        };
        arrpush(obj->sections, section);
        section_header_table_offset += sizeof(ELFSectionHeader);
    }

    /* collect string table (section and symbol) */
    obj->section_string_table = obj->sections + obj->elf_header.e_shstrndx;
    const Section *symbol_section = object_get_section_by_type(obj, SHT_SYMTAB);
    obj->symbol_string_table = obj->sections + symbol_section->header.sh_link;

    /* collect section name */
    arrforeach(Section, obj->sections) {
        const char *section_name = obj->section_string_table->content.data + it->header.sh_name;
        it->name = svfromcstr(section_name);
    }

    /* collect symbol */
    const char *symbol_table_offset = obj->content + symbol_section->header.sh_offset;
    int num_symbols = symbol_section->header.sh_size / sizeof(ELFSymbol);
    for (int i = 0; i < num_symbols; i++) {
        ELFSymbol symbol_header = *(ELFSymbol *) symbol_table_offset;
        const char *symbol_name = obj->symbol_string_table->content.data + symbol_header.st_name;
        Symbol symbol = {
            .header = symbol_header,
            .name = svfromcstr(symbol_name),
        };
        arrpush(obj->symbols, symbol);
        symbol_table_offset += sizeof(ELFSymbol);
    }
}

void parse_symbol(Object *obj)
{
    (void) obj;
}
