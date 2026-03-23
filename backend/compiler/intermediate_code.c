#include "intermediate_code.h"
#include <stdlib.h>
#include <string.h>

#define ICG_MAX_LINES 1024

static char *icg_lines[ICG_MAX_LINES];
static int icg_count = 0;

void icg_reset(void) {
    int i;
    for (i = 0; i < icg_count; i++) {
        free(icg_lines[i]);
        icg_lines[i] = NULL;
    }
    icg_count = 0;
}

void icg_emit(const char *line) {
    if (!line || icg_count >= ICG_MAX_LINES) {
        return;
    }
    icg_lines[icg_count++] = strdup(line);
}

void icg_dump(FILE *out) {
    int i;
    FILE *target = out ? out : stdout;

    fprintf(target, "=== INTERMEDIATE CODE (TRACE LOG) ===\n");
    for (i = 0; i < icg_count; i++) {
        fprintf(target, "%s\n", icg_lines[i]);
    }
    fprintf(target, "=== END INTERMEDIATE CODE ===\n");
}
