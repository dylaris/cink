#include "cink.h"

Object *object_create(const char *filename)
{
    log(CORVUS_INFO, "create an object: %s", filename);
    Object *obj = malloc(sizeof(Object));
    if (!obj) return NULL;
    obj->filename = filename;
    obj->content = read_entire_file(filename, &obj->size);
    obj->sections = NULL;
    return obj;
}

bool object_parse(Object *obj)
{
    log(CORVUS_INFO, "validate the elf magic number: %s", obj->filename);
    if (!validate(obj)) return false;

    log(CORVUS_INFO, "parse the elf header: %s", obj->filename);
    parse_elf_header(obj);

    log(CORVUS_INFO, "parse the section: %s", obj->filename);
    parse_section_header(obj);
    log(CORVUS_INFO, "collect [%d] sections: %s", arrlen(obj->sections), obj->filename);

    log(CORVUS_INFO, "parse the symbol: %s", obj->filename);
    parse_symbol(obj);

    return true;
}

void object_destroy(Object *obj)
{
    log(CORVUS_INFO, "destroy an object: %s", obj->filename);
    free(obj->content);
    if (obj->sections) arrfree(obj->sections);
    free(obj);
}
