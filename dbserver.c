/*
 * dbserver.c
 *      CSSE2310 - Assignment Three - Semester One, 2022
 *
 *      Written by William Sawyer, w.sawyer@uqconnect.edu.au
 *
 * Usage:
 *      dbserver authfile connections [portnum]
 */

#include "dbserver.h"

#include <csse2310a4.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MIN_ARGS 3

enum ExitCode {
    EXIT_INVALID_COMMAND = 1
};

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

    return EXIT_SUCCESS;
}

void validate_arguments(int argc, char* argv[]) {
    if (argc < MIN_ARGS
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

    return strlen(arg) ? true : false; // only return true if arg is non-empty
}

