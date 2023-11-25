#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "error.h"

#define _FOPEN(fp, fp_name, mode)               \
    FILE *fp = fopen (fp_name, mode);           \
    if (fp == NULL) {return ERR_FOPEN;}

#define _FCLOSE(fp)                             \
    if (fclose (fp) != 0) {return ERR_FCLOSE;}  \

size_t get_file_size (FILE *stream);

#endif //UTILS_H