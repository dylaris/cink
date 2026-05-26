#include "cink.h"

int main(void)
{
    Object *obj = object_create("test/hello.o");

    object_parse(obj);

    arrforeach(Section, obj->sections) {
        printf("[%d]: '"svfmt"'\n", arroff(obj->sections, it), svarg(it->name));
    }
    arrforeach(Symbol, obj->symbols) {
        printf("[%d]: '"svfmt"'\n", arroff(obj->symbols, it), svarg(it->name));
    }

    object_destroy(obj);
    return 0;
}
