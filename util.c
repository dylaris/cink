#include "cink.h"

char *read_entire_file(const char *filename, size_t *sz)
{
    FILE *f = NULL;
    char *buffer = NULL;

    f = fopen(filename, "rb");
    if (!f) goto err;

    fseek(f, 0, SEEK_END);
    size_t size = (size_t) ftell(f);
    rewind(f);

    buffer = malloc(size + 1);
    if (!buffer) goto err;
    if (fread(buffer, 1, size, f) != size) goto err;
    buffer[size] = '\0';
    *sz = size;

    fclose(f);
    return buffer;

err:
    *sz = 0;
    if (f) fclose(f);
    if (buffer) free(buffer);
    return NULL;
}

