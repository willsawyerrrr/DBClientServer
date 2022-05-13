#include "dbclient.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define EXIT_INSUFFICIENT_ARGS 1
#define EXIT_INVALID_KEY 1

int main(int argc, char* argv[]) {
    // validate arguments
    if (!validate_num_arguments(argc)) {
        return EXIT_INSUFFICIENT_ARGS;
    } elif (!validate_key(argv[1])) {
        return EXIT_INVALID_KEY;
    }

    // connect to server

    // GET or PUT key/value pair

    return EXIT_SUCCESS;
}

bool validate_num_arguments(int argc) {
     if (argc < 3) {
        fprintf(stderr, "Usage: dbclient portnum key [value]\n");
        fflush(stderr);
        return false;
    }
     return true;
}

bool validate_key(char* key) {
    for (int i = 0; key[i]; i++) {
        if (key[i] == ' ' || key[i] == '\n') {
            fprintf(stderr,
                    "dbclient: key must not contain spaces or newlines\n");
            fflush(stderr);
            return false;
    }
    return true;
}

