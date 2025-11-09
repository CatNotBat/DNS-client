#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "dns_protocol.h"
#include "utils.h"


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

void parse_dns_response(const uint8_t* response_buffer, size_t response_length) {
    DNSHeader* header = (DNSHeader*)response_buffer;
    uint16_t ancount = ntohs(header->ancount);
    const uint8_t* ptr = response_buffer + sizeof(DNSHeader);
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