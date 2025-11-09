#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_DOMAINS 64
#define MAX_LINE_LENGTH 255

typedef struct {
    char* TLD;
    char* SLD;
    char* subdomain;
} Domain;

int read_lines_from_file(char* file_content[], int max_lines, FILE* fp);

void split_line_to_domain(const char* line, Domain* domain);
void free_domain_strings(Domain* domain); // Helper to clean up mallocs

#endif