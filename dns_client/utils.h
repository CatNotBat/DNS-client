#ifndef UTILS_H
#define UTILS_H

#define MAX_LINE_LEN 256

#include <stdio.h>

int read_lines_from_file(char* line_pointers[], int max_lines, FILE *fp);

#endif