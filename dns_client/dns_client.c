#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
#include <string.h> 
#include "dns_client.h"
#include "utils.h"

#define MAX_DOMAINS 64

typedef struct 
{
    char* TLD;
    char* SLD;
    char* subdomain;
    
}Domain;

typedef struct {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
}DNSHeader;


void split_line_to_domain(const char* line, Domain* domain){
    const char *delimiter = ".";
    char *token;
    size_t length = strlen(line) + 1;
    char *line_copy = (char *)malloc(length * sizeof(char));
    strcpy(line_copy, line);
    
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

int main() {
    FILE *fp;
    
    char* file_content[MAX_DOMAINS]; 
    int line_count = 0;

    fp = fopen("text.txt", "r");
    line_count = read_lines_from_file(file_content, MAX_DOMAINS, fp);
    fclose(fp);

    Domain domains[MAX_DOMAINS];
    for (int i = 0; i < line_count; i++) {
        split_line_to_domain(file_content[i], &domains[i]);
        printf("Domain: %s, SLD: %s, TLD: %s\n", 
               domains[i].subdomain ? domains[i].subdomain : "N/A",
               domains[i].SLD ? domains[i].SLD : "N/A",
               domains[i].TLD ? domains[i].TLD : "N/A");

        
        
        free(file_content[i]);
        if (domains[i].TLD) free(domains[i].TLD);
        if (domains[i].SLD) free(domains[i].SLD);
        if (domains[i].subdomain) free(domains[i].subdomain);
    }



    return 0;
}