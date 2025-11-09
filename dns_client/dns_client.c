#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "dns_client.h"
#include "utils.h"

#define MAX_DOMAINS 64


int main() {
    FILE *fp;
    
    char* file_content[MAX_DOMAINS]; 
    int line_count = 0;

    fp = fopen("text.txt", "r");
    line_count = read_lines_from_file(file_content, MAX_DOMAINS, fp);
    fclose(fp);
    printf("--- File Content (%d lines) ---\n", line_count);
    for (int i = 0; i < line_count; i++) {
        printf("Line %d: %s\n", i, file_content[i]);
    }
    printf("------------------------------\n");


    for (int i = 0; i < line_count; i++) {
        free(file_content[i]);
    }

    return 0;
}