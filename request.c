#include "request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILENAME_SIZE 15

FILE* construct_HTTP_request(char* action, char* database, char* key,
        char* value) {
    char* filename = malloc(sizeof(char) * FILENAME_SIZE);
    sprintf(filename, "request%d", getpid()); // request file unique to process

    FILE* request = fopen(filename, "w");
    fprintf(request, "%s /%s/%s HTTP/1.1\r\n", action, database, key);
    
    if (value) {
        fprintf(request, "Content-Length: %ld\r\n", strlen(value) + 1);
        fprintf(request, "%s", value);
    } else {
        fprintf(request, "\r\n");
    }

    fclose(request);

    return fopen(filename, "r");
}

