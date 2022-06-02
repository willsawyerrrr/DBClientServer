#include "address.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_CANNOT_CONNECT 2

// Inspired by net1.c, shown in week 9's contact
struct sockaddr* get_addr(char* port) {
    struct addrinfo* info = NULL;

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          // want to use IPv4
    hints.ai_socktype = SOCK_STREAM;    // want to use TCP

    if (getaddrinfo("localhost", port, &hints, &info)) {
        // could not get an address
        freeaddrinfo(info);
        return NULL;
    }

    // get and return generic socket address member
    return info->ai_addr;
}
