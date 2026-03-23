#include "intermediate_code.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define ICG_MAX_LINES 8192

static char *icg_lines[ICG_MAX_LINES];
static int icg_count = 0;
static int icg_temp_counter = 0;
static int icg_label_counter = 0;

void icg_reset(void) {
    int i;
    for (i = 0; i < icg_count; i++) {
        free(icg_lines[i]);
        icg_lines[i] = NULL;
    }
    icg_count = 0;
    icg_temp_counter = 0;
    icg_label_counter = 0;
}

void icg_emit(const char *line) {
    if (!line || icg_count >= ICG_MAX_LINES) {
        return;
    }
    icg_lines[icg_count++] = strdup(line);
}

void icg_emitf(const char *fmt, ...) {
    char buffer[1024];
    va_list args;

    if (!fmt) {
        return;
    }

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    icg_emit(buffer);
}

char *icg_new_temp(void) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "$t%d", ++icg_temp_counter);
    return strdup(buffer);
}

char *icg_new_label(void) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "L%d", ++icg_label_counter);
    return strdup(buffer);
}

void icg_dump(FILE *out) {
    int i;
    FILE *target = out ? out : stdout;

    fprintf(target, ";; EmotionScript TAC\n");
    for (i = 0; i < icg_count; i++) {
        fprintf(target, "%s\n", icg_lines[i]);
    }
}

int icg_write_to_file(const char *path) {
    FILE *fp;
    if (!path) {
        return 1;
    }

    fp = fopen(path, "w");
    if (!fp) {
        return 1;
    }

    icg_dump(fp);
    fclose(fp);
    return 0;
}
