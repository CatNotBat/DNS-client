#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include "dns_client.h"
#include "utils.h"

#define MAX_DOMAINS 64
#define PACKET_SIZE 512
#define QTYPE_AND_QCLASS_SIZE 4
#define DNS_ANSWER_NAME_SIZE 2
#define DNS_ANSWER_TYPE_SIZE 2
#define DNS_ANSWER_CLASS_SIZE 2
#define DNS_ANSWER_TTL_SIZE 4
#define DNS_ANSWER_DATA_LENGTH_SIZE 2
#define A 1
#define AAAA 28


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

void make_dns_query(const Domain* domain, uint8_t* buffer, size_t* query_length, uint16_t qtype) {
    DNSHeader header;
    header.id = htons(0x1234); 
    header.flags = htons(0x0100); 
    header.qdcount = htons(1); 
    header.ancount = 0;
    header.nscount = 0;
    header.arcount = 0;

    size_t offset = 0;
    memcpy(buffer + offset, &header, sizeof(DNSHeader));
    offset += sizeof(DNSHeader);

    const char* labels[] = {domain->subdomain, domain->SLD, domain->TLD};
    for (int i = 0; i < 3; i++) {
        if (labels[i] != NULL) {
            size_t label_length = strlen(labels[i]);
            buffer[offset++] = (uint8_t)label_length; 
            memcpy(buffer + offset, labels[i], label_length);
            offset += label_length;
        }
    }
    buffer[offset++] = 0; 

    uint16_t qtype_net = htons(qtype); 
    uint16_t qclass = htons(1); 
    memcpy(buffer + offset, &qtype_net, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    memcpy(buffer + offset, &qclass, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    *query_length = offset;
}

size_t send_dns_query(const uint8_t* query, size_t query_length, uint8_t* response_buffer, size_t buffer_size) {
    struct sockaddr_in address;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0;

    address.sin_family = AF_INET;
    address.sin_port = htons(53);
    address.sin_addr.s_addr = inet_addr("8.8.8.8");
    connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    send(sockfd, query, query_length, 0);
    ssize_t response_length = recv(sockfd, response_buffer, buffer_size, 0);
    if (response_length < 0) {
        perror("Receive failed");
    } else {
        printf("Received %zd bytes in response\n", response_length);
    }
    close(sockfd);
    return response_length;
}

void parse_dns_response(const uint8_t* response_buffer, size_t response_length) {
    DNSHeader* header = (DNSHeader*)response_buffer;
    uint16_t ancount = ntohs(header->ancount);
    uint8_t* ptr = response_buffer + sizeof(DNSHeader);
    while (ptr < response_buffer + response_length) {
        if (*ptr == 0) {
            ptr++;
            break;
        }
        ptr += (*ptr) + 1;
    }
    ptr += QTYPE_AND_QCLASS_SIZE;

    for (int i = 0; i < ancount; i++) {
        ptr += DNS_ANSWER_NAME_SIZE;
        uint16_t type = ntohs(*(uint16_t*)ptr);
        ptr += DNS_ANSWER_TYPE_SIZE;
        ptr += DNS_ANSWER_CLASS_SIZE;
        ptr += DNS_ANSWER_TTL_SIZE;
        uint16_t data_length = ntohs(*(uint16_t*)ptr);
        ptr += DNS_ANSWER_DATA_LENGTH_SIZE;
        if (type == 1 && data_length == 4) {
             printf("IP Address: %u.%u.%u.%u\n", ptr[0], ptr[1], ptr[2], ptr[3]);
        }
        else if (type == 28 && data_length == 16) {
            printf("IP Address (IPv6): ");
            uint16_t* seg_ptr = (uint16_t*)ptr;
            for (int j = 0; j < 8; j++) {
                printf("%x", ntohs(seg_ptr[j]));
                if (j < 7) {
                    printf(":");
                }
            }
            printf("\n");
        }
        ptr += data_length;
    }
    



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
        uint8_t query_packet_buffer[PACKET_SIZE];
        uint8_t response_packet_buffer[PACKET_SIZE];
        size_t query_length = 0;

        split_line_to_domain(file_content[i], &domains[i]);
        make_dns_query(&domains[i], query_packet_buffer, &query_length, A);
        printf("DNS Query Packet for %s (length: %zu bytes):\n", file_content[i], query_length);
        size_t response_length = send_dns_query(query_packet_buffer, query_length, response_packet_buffer, PACKET_SIZE);
        parse_dns_response(response_packet_buffer, response_length);

        
        
        free(file_content[i]);
        if (domains[i].TLD) free(domains[i].TLD);
        if (domains[i].SLD) free(domains[i].SLD);
        if (domains[i].subdomain) free(domains[i].subdomain);
    }



    return 0;
}