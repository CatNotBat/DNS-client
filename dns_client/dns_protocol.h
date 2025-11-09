#ifndef DNS_PROTOCOL_H
#define DNS_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include "utils.h"

#define PACKET_SIZE 512
#define QTYPE_AND_QCLASS_SIZE 4
#define DNS_ANSWER_NAME_SIZE 2
#define DNS_ANSWER_TYPE_SIZE 2
#define DNS_ANSWER_CLASS_SIZE 2
#define DNS_ANSWER_TTL_SIZE 4
#define DNS_ANSWER_DATA_LENGTH_SIZE 2
#define A 1
#define AAAA 28

typedef struct {
    uint16_t id;
    uint16_t flags;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
} DNSHeader;

void make_dns_query(const Domain* domain, uint8_t* buffer, size_t* query_length, uint16_t qtype);
void parse_dns_response(const uint8_t* response_buffer, size_t response_length);

#endif