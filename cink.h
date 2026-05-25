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
    const char *filename;
    char *content;
    size_t size;
    ELFHeader elf_header;
    Array(Section) sections;
} Object;

Object *object_create(const char *filename);
void object_destroy(Object *obj);
bool object_parse(Object *obj);

/* parse.c */

bool validate(const Object *obj);
void parse_elf_header(Object *obj);
void parse_section(Object *obj);
void parse_symbol(Object *obj);

/* util.c */

char *read_entire_file(const char *filename, size_t *sz);

#endif /* CINK_H */
