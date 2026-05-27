#ifndef CINK_H
#define CINK_H

#include "corvus.h"
#include "types.h"
#include "elf.h"

/* object.c */

typedef struct {
    const char *filename;
    char *content;
    size_t size;
    ELFHeader ehdr;                /* elf header */
    Array(ELFSectionHeader) shdrs; /* section headers */
    Array(ELFSymbol) syms;         /* symbols */
    const char *shstroff;          /* section header string offset */
    const char *symstroff;         /* symbol string offset */
} Object;

Object *object_create(const char *filename);
void object_destroy(Object *obj);
bool object_parse(Object *obj);
const ELFSectionHeader *object_get_shdr(const Object *obj, u32 type);
StrView object_get_secbytes(const Object *obj, int index);
StrView object_get_secname(const Object *obj, int index);
StrView object_get_symanme(const Object *obj, int index);

/* parse.c */

bool validate(const Object *obj);
void parse_elf_header(Object *obj);
void parse_section(Object *obj);
void parse_symbol(Object *obj);

/* context.c */

typedef struct {
    const char *output_path;
    Array(const char *) lib_paths;
    const char *arch;
    int argc;
    char **argv;
} Context;

Context *context_create(int argc, char **argv);
void context_destroy(Context *ctx);

/* util.c */

char *read_entire_file(const char *filename, size_t *sz);

#endif /* CINK_H */
