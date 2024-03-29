/*
 * dbclient.c
 *      CSSE2310 - Assignment Three - Semester One, 2022
 *
 *      Written by William Sawyer, w.sawyer@uqconnect.edu.au
 *
 * Usage:
 *      dbclient portnum key [value]
 */

#include "address.h"
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
    EXIT_CANNOT_CONNECT = 2,
    EXIT_GET_FAILED = 3,
    EXIT_PUT_FAILED = 4
};

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

    int fd = establish_connection(argv[1]);

    int fd2 = dup(fd);
    FILE* read = fdopen(fd, "r");
    FILE* write = fdopen(fd2, "w");
    
    char* request = NULL;
    char* action = malloc(4);
    if (argc > 3) {     // value specified - PUT request
        strcpy(action, "PUT");
    } else {            // value not specified - GET request
        strcpy(action, "GET");
    }
    request = construct_HTTP_request(action, "public", argv[2], argv[3]);

    // send request
    fprintf(write, "%s", request);
    fflush(write);
    fclose(write);
    free(request);

    int exitCode = handle_response(read, action);

    // close connection
    fclose(read);
    return exitCode;
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

    if (!address || connect(socketDes, address, sizeof(struct sockaddr_in))) {
        // if address is NULL or could not connect
        fprintf(stderr, "dbclient: unable to connect to port %s\n", port);
        fflush(stderr);
        exit(EXIT_CANNOT_CONNECT);
    }

    return socketDes;
}

int handle_response(FILE* stream, char* action) {
    int status;
    char* statusexplanation;
    HttpHeader** headers;
    char* body;

    if (get_HTTP_response(stream, &status, &statusexplanation, &headers,
            &body) && status == 200) {
        // OK
        if (!strncmp(action, "GET", 4)) {
            printf("%s\n", body);
            fflush(stdout);
        }
        return EXIT_SUCCESS;
    } else {
        // not OK
        if (!strncmp(action, "GET", 4)) {
            return EXIT_GET_FAILED;
        } else {
            return EXIT_PUT_FAILED;
        }
    }
        
}

