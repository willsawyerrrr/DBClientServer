/*
 * dbclient.c
 *      CSSE2310 - Assignment Three - Semester One, 2022
 *
 *      Written by William Sawyer, w.sawyer@uqconnect.edu.au
 *
 * Usage:
 *      dbclient portnum key [value]
 * Key must not contain any spaces or newline characters.
 */

#include "dbclient.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

enum ExitCode {
    EXIT_INSUFFICIENT_ARGS = 1,
    EXIT_INVALID_KEY = 1
};

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

    // connect to server

    // GET or PUT key/value pair

    return EXIT_SUCCESS;
}

void validate_arguments(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: dbclient portnum key [value]\n");
        fflush(stderr);
        exit(EXIT_INSUFFICIENT_ARGS);
    } else if (!validate_key(argv[1])) {
        fprintf(stderr, "dbclient: key must not contain spaces or newlines\n");
        fflush(stderr);
        exit(EXIT_INVALID_KEY);
    }
}

bool validate_key(char* key) {
    for (int i = 0; key[i]; i++) {
        if (key[i] == ' ' || key[i] == '\n') {
            return false;
        }
    }
    return true;
}

