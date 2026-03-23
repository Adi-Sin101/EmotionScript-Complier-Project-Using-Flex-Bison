#ifndef INTERMEDIATE_CODE_H
#define INTERMEDIATE_CODE_H

#include <stdio.h>

void icg_reset(void);
void icg_emit(const char *line);
void icg_emitf(const char *fmt, ...);
char *icg_new_temp(void);
char *icg_new_label(void);
void icg_dump(FILE *out);
int icg_write_to_file(const char *path);

#endif
