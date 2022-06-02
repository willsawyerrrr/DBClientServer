/*
 * dbserver.c
 *      CSSE2310 - Assignment Three - Semester One, 2022
 *
 *      Written by William Sawyer, w.sawyer@uqconnect.edu.au
 *
 * Usage:
 *      dbserver authfile connections [portnum]
 */

#include "address.h"
#include "dbserver.h"

#include <csse2310a4.h>
#include <netdb.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define MIN_ARGS 3
#define MAX_ARGS 4

// macro instead of Enum since more exist where external functions: i.e. those
// in the header 'address.h'; may cause the process to exit.
#define EXIT_INVALID_COMMAND 1
#define EXIT_CANNOT_AUTHENTICATE 2
#define EXIT_CANNOT_LISTEN 3

#define AUTHFILE_ARG 1
#define CONNECTIONS_ARG 2
#define PORTNUM_ARG 3

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

    FILE* authfile = get_authfile(argv[AUTHFILE_ARG]);

    int connections = atoi(argv[CONNECTIONS_ARG]);
    char* port = argv[PORTNUM_ARG] ? argv[PORTNUM_ARG] : "0";
    int server = begin_listening(port, connections);

    int portnum = get_portnum(server);
    fprintf(stderr, "%d\n", portnum);
    fflush(stderr);
 
    return EXIT_SUCCESS;
}

void validate_arguments(int argc, char* argv[]) {
    if (argc < MIN_ARGS || argc > MAX_ARGS
            || !validate_connections(argv[2])
            || (argc > 3 && !validate_portnum(argv[3]))) {
        fprintf(stderr, "Usage: dbserver authfile connections [portnum]\n");
        fflush(stderr);
        exit(EXIT_INVALID_COMMAND);
    }
}

bool validate_connections(char* arg) {
    int connections = atoi(arg);
    return (validate_integral_arg(arg) && connections >= 0);
}

bool validate_portnum(char* arg) {
    int portnum = atoi(arg);
    return (validate_integral_arg(arg)
            && (!portnum || (portnum >= 1024 && portnum <= 65535)));
}

bool validate_integral_arg(char* arg) {
    for (int i = 0; arg[i]; i++) {
        if (!isdigit(arg[i])) {
            return false;
        }
    }

    return strlen(arg); // only return true if arg is non-empty
}

FILE* get_authfile(char* filename) {
    FILE* authfile = fopen(filename, "r");

    if (!authfile) {
        fprintf(stderr, "dbserver: unable to read authentication string\n");
        fflush(stderr);
        exit(EXIT_CANNOT_AUTHENTICATE);
    }

    return authfile;
}

int begin_listening(char* port, int connections) {
    struct sockaddr* address = get_addr(port);
    int server = socket(AF_INET, SOCK_STREAM, 0);

    int optVal = 1;
    if (!address
            || setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optVal,
                    sizeof(int))
            || bind(server, address, sizeof(struct sockaddr))
            || listen(server, connections)) {
        // error trying to get address info, set socket options, bind or listen
        fprintf(stderr, "dbserver: unable to open socket for listening\n");
        fflush(stderr);
        exit(EXIT_CANNOT_LISTEN);
    }

    return server;
}

int get_portnum(int server) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    socklen_t size = sizeof(address);
    if (getsockname(server, (struct sockaddr*) &address, &size)) {
        return 0;
    }

    return htons(address.sin_port); // convert to network-byte order
}

