#include "utils.h"
#include <string.h>
#include <stdlib.h>

int read_lines_from_file(char* line_pointers[], int max_lines, FILE *fp) {
    char temp_buffer[MAX_LINE_LENGTH];
    int i = 0;

    while (i < max_lines && fgets(temp_buffer, MAX_LINE_LENGTH, fp) != NULL) {
        temp_buffer[strcspn(temp_buffer, "\n")] = '\0';
        line_pointers[i] = malloc(strlen(temp_buffer) + 1);
        strcpy(line_pointers[i], temp_buffer);
        i++;
    }
    return i;
}

void split_line_to_domain(const char* line, Domain* domain) {
    const char *delimiter = ".";
    char *token;
    size_t length = strlen(line) + 1;
    char *line_copy = (char *)malloc(length * sizeof(char));
    strcpy(line_copy, line);

    domain->subdomain = NULL;
    domain->SLD = NULL;
    domain->TLD = NULL;

    token = strtok(line_copy, delimiter);
    if (token != NULL) {
        domain->subdomain = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(domain->subdomain, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL) {
        domain->SLD = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(domain->SLD, token);
    }

    token = strtok(NULL, delimiter);
    if (token != NULL) {
        domain->TLD = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(domain->TLD, token);
    }

    free(line_copy);
}

void free_domain_strings(Domain* domain){
    if (domain->TLD) free(domain->TLD);
    if (domain->SLD) free(domain->SLD);
    if (domain->subdomain) free(domain->subdomain);
}