#include "interpreter.h"
#include <stdarg.h>

void interpreter_trace(FILE *out, const char *fmt, ...) {
    va_list args;
    FILE *target = out ? out : stdout;

    va_start(args, fmt);
    fprintf(target, "[TRACE] ");
    vfprintf(target, fmt, args);
    fprintf(target, "\n");
    va_end(args);
}
