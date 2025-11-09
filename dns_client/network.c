#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "network.h"


size_t send_dns_query(const uint8_t* query, size_t query_length, uint8_t* response_buffer, size_t buffer_size) {
    struct sockaddr_in address;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
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
        return -1;

    } else {
        printf("Received %zd bytes in response\n", response_length);
    }
    close(sockfd);

    return response_length;
}