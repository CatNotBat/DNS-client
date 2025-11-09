#ifndef NETWORK_H
#define NETWORK_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h> // For ssize_t

ssize_t send_dns_query(const uint8_t* query, size_t query_length, uint8_t* response_buffer, size_t buffer_size);

#endif