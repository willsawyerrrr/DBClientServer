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

#include <csse2310a4.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FILENAME_SIZE 15

enum ExitCode {
    EXIT_INSUFFICIENT_ARGS = 1,
    EXIT_INVALID_KEY = 1
};

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

    // connect to server

    if (argc > 3) { // PUT request
    
    } else { // GET request
    
    }

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
