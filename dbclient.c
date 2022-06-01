/*
 * dbclient.c
 *      CSSE2310 - Assignment Three - Semester One, 2022
 *
 *      Written by William Sawyer, w.sawyer@uqconnect.edu.au
 *
 * Usage:
 *      dbclient portnum key [value]
 */

#include "dbclient.h"
#include "request.h"

#include <csse2310a4.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

enum ExitCode {
    EXIT_INSUFFICIENT_ARGS = 1,
    EXIT_INVALID_KEY = 1,
    EXIT_CANNOT_CONNECT = 2
};

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

    int fd = establish_connection(argv[1]);

    int fd2 = dup(fd);
    FILE* read = fdopen(fd, "r");
    FILE* write = fdopen(fd2, "w");
    
    char* action = malloc(4);
    if (argc > 3) {     // value specified - PUT request
        strncpy(action, "PUT\0", 4);   // 4 bytes to include '\0'
    } else {            // value not specified - GET request
        strncpy(action, "GET\0", 4);
    }
    char* request = construct_HTTP_request(action, "public", argv[2], argv[3]);

    // close connection
    fclose(read);
    fclose(write);
    return EXIT_SUCCESS;
}

void validate_arguments(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: dbclient portnum key [value]\n");
        fflush(stderr);
        exit(EXIT_INSUFFICIENT_ARGS);
    }
     
    validate_key(argv[2]);
}

void validate_key(char* key) {
    for (int i = 0; key[i]; i++) {
        if (key[i] == ' ' || key[i] == '\n') {
            fprintf(stderr,
                    "dbclient: key must not contain spaces or newlines\n");
            fflush(stderr);
            exit(EXIT_INVALID_KEY);
        }
    }
}

int establish_connection(char* port) {
    struct sockaddr* address = get_addr(port);

    int socketDes = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(socketDes, address, sizeof(struct sockaddr_in))) {
        // could not connect
        fprintf(stderr, "dbclient: unable to connect to port %s\n", port);
        fflush(stderr);
        exit(EXIT_CANNOT_CONNECT);
    }

    return socketDes;
}

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
        fprintf(stderr, "dbclient: unable to connect to port %s\n", port);
        fflush(stderr);
        exit(EXIT_CANNOT_CONNECT);
    }

    // return generic socket address member
    return info->ai_addr;
}

