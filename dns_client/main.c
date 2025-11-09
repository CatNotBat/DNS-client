#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "dns_protocol.h"
#include "network.h"



int main() {
    FILE *fp;
    
    char* file_content[MAX_DOMAINS]; 
    int line_count = 0;

    fp = fopen("domain_name_list.txt", "r");
    line_count = read_lines_from_file(file_content, MAX_DOMAINS, fp);
    fclose(fp);

    Domain domains[MAX_DOMAINS];
    for (int i = 0; i < line_count; i++) {
        uint8_t query_packet_buffer[PACKET_SIZE];
        uint8_t response_packet_buffer[PACKET_SIZE];
        size_t query_length = 0;

        split_line_to_domain(file_content[i], &domains[i]);
        make_dns_query(&domains[i], query_packet_buffer, &query_length, A);
        printf("DNS Query Packet for %s (length: %zu bytes):\n", file_content[i], query_length);
        ssize_t response_length = send_dns_query(query_packet_buffer, query_length, response_packet_buffer, PACKET_SIZE);
        if (response_length > 0) {
            parse_dns_response(response_packet_buffer, (size_t)response_length);
        }

        
        
        free_domain_strings(&domains[i]);
        free(file_content[i]);
        printf("---\n");
    }

    return 0;
}