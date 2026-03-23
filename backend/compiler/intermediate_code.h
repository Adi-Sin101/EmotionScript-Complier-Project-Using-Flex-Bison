#ifndef INTERMEDIATE_CODE_H
#define INTERMEDIATE_CODE_H

#include <stdio.h>

void icg_reset(void);
void icg_emit(const char *line);
void icg_dump(FILE *out);

#endif
