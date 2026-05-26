#ifndef CINK_H
#define CINK_H

#include "corvus.h"
#include "types.h"
#include "elf.h"

/* object.c */

typedef struct {
    ELFSectionHeader header;
    StrView name;
    StrView content;
} Section;

typedef struct {
    ELFSymbol header;
    StrView name;
} Symbol;

typedef struct {
    const char *filename;
    char *content;
    size_t size;

    ELFHeader elf_header;

    Array(Section) sections;
    const Section *section_string_table;
    const Section *symbol_string_table;

    Array(Symbol) symbols;
} Object;

Object *object_create(const char *filename);
void object_destroy(Object *obj);
bool object_parse(Object *obj);
const Section *object_get_section_by_type(const Object *obj, u32 type);

/* parse.c */

bool validate(const Object *obj);
void parse_elf_header(Object *obj);
void parse_section(Object *obj);
void parse_symbol(Object *obj);

/* util.c */

char *read_entire_file(const char *filename, size_t *sz);

#endif /* CINK_H */
