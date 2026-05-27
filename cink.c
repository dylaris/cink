#include "cink.h"

int main(int argc, char **argv)
{
    // for (int i = 0; i < argc; i++) {
    //     printf("%s\n", argv[i]);
    // }
    // return 1;

    Object *obj = object_create("test/hello.o");

    object_parse(obj);

    arrforeach(ELFSectionHeader, obj->shdrs) {
        int index = arroff(obj->shdrs, it);
        StrView secname = object_get_secname(obj, index);
        printf("[%d]: '"svfmt"'\n", index, svarg(secname));
    }
    arrforeach(ELFSymbol, obj->syms) {
        int index = arroff(obj->syms, it);
        StrView symname = object_get_symname(obj, index);
        printf("[%d]: '"svfmt"'\n", index, svarg(symname));
    }

    object_destroy(obj);
    return 0;
}
