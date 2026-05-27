#include "cink.h"

Context *context_create(int argc, char **argv)
{
    log(CORVUS_INFO, "create an context");
    Context *ctx = malloc(sizeof(Context));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(Context));
    ctx->argc = argc;
    ctx->argv = argv;
    return ctx;
}

void context_destroy(Context *ctx)
{
    log(CORVUS_INFO, "destroy an context");
    if (ctx->lib_paths) free(ctx->lib_paths);
    free(ctx);
}

void context_parse(Context *ctx)
{
}
