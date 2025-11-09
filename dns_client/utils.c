#include "utils.h"


int read_lines_from_file(char* line_pointers[], int max_lines, FILE *fp) {
    char temp_buffer[MAX_LINE_LEN];
    int i = 0;

    while (i < max_lines && fgets(temp_buffer, MAX_LINE_LEN, fp) != NULL) {
        temp_buffer[strcspn(temp_buffer, "\n")] = '\0';
        line_pointers[i] = malloc(strlen(temp_buffer) + 1);
        strcpy(line_pointers[i], temp_buffer);
        i++;
    }
    return i;
}