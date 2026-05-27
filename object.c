#include "cink.h"

Object *object_create(const char *filename)
{
    log(CORVUS_INFO, "%s: create an object", filename);
    Object *obj = malloc(sizeof(Object));
    if (!obj) return NULL;
    memset(obj, 0, sizeof(Object));
    obj->filename = filename;
    obj->content = read_entire_file(filename, &obj->size);
    return obj;
}

bool object_parse(Object *obj)
{
    log(CORVUS_INFO, "%s: validate the elf magic number", obj->filename);
    if (!validate(obj)) return false;

    log(CORVUS_INFO, "%s: parse the elf header", obj->filename);
    parse_elf_header(obj);

    log(CORVUS_INFO, "%s: parse the section", obj->filename);
    parse_section(obj);
    log(CORVUS_INFO, "%s: collect [%d] sections", obj->filename, arrlen(obj->shdrs));

    log(CORVUS_INFO, "%s: parse the symbol", obj->filename);
    parse_symbol(obj);
    log(CORVUS_INFO, "%s: collect [%d] symbols", obj->filename, arrlen(obj->syms));

    return true;
}

void object_destroy(Object *obj)
{
    log(CORVUS_INFO, "%s: destroy an object", obj->filename);
    free(obj->content);
    if (obj->shdrs) arrfree(obj->shdrs);
    if (obj->syms) arrfree(obj->syms);
    free(obj);
}

const ELFSectionHeader *object_get_shdr(const Object *obj, u32 type)
{
    arrforeach(ELFSectionHeader, obj->shdrs) {
        if (it->sh_type == type) {
            return it;
        }
    }
    return NULL;
}

StrView object_get_secbytes(const Object *obj, int index)
{
    const ELFSectionHeader *shdr = &obj->shdrs[index];
    return (StrView) {
        .data = obj->content + shdr->sh_offset,
        .len = shdr->sh_size,
    };
}

StrView object_get_secname(const Object *obj, int index)
{
    const ELFSectionHeader *shdr = &obj->shdrs[index];
    return svfromcstr(obj->shstroff + shdr->sh_name);
}

StrView object_get_symname(const Object *obj, int index)
{
    const ELFSymbol *sym = &obj->syms[index];
    return svfromcstr(obj->symstroff + sym->st_name);
}
