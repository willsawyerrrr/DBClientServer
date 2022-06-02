#include "request.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Number of characters common across HTTP request lines:
 *  - space between action and database;        (1 char)
 *  - two slashes in database;                  (2 chars)
 *  - space between database and HTTP version:  (1 char)
 *  - HTTP version;                             (8 chars)
 *  - CRLF sequence; and                        (2 chars)
 *  - null terminator.                          (1 char)
 */
#define REQUEST_LINE_TEMPLATE_SIZE 15

/*
 * Number of characters common across HTTP Content-Length headers:
 * - "Content-Length: " prefix;                 (16 chars)
 * - header line CRLF sequence; and             (2 chars)
 * - blank line CRLF sequence.                  (2 chars)
 *
 * NOTE: A null terminator is already present from the template size.
 *       As such, no space is allocated here.
 */
#define CONTENT_LENGTH_TEMPLATE_SIZE 20

#define CRLF_SIZE 2

char* construct_HTTP_request(char* action, char* database, char* key,
        char* value) {
    // add data size to template size to get total size for memory allocation
    int requestSize = REQUEST_LINE_TEMPLATE_SIZE + strlen(action)
            + strlen(database) + strlen(key);
    // create buffer to hold request
    char* request = malloc(requestSize);
    
    sprintf(strchr(request, '\0'), "%s /%s/%s HTTP/1.1\r\n", action, database, key);
    
    if (value) {
        int valueLength = strlen(value);
        // length of decimal string representing length of given value
        // i.e. the number of characters required to add the Content-Length
        int strLength = log10(valueLength) + 1;

        // already have space for a null terminator - no need to add it again,
        // but we do need to add space for the prefix, the content length
        // number and the content itself
        requestSize += (CONTENT_LENGTH_TEMPLATE_SIZE + strLength
                + valueLength);

        request = realloc(request, requestSize);
        sprintf(strchr(request, '\0'), "Content-Length: %d\r\n\r\n", valueLength);
        sprintf(strchr(request, '\0'), "%s", value);
    } else {
        // allocate space for CRLF sequence to terminate header line
        requestSize += CRLF_SIZE;
        request = realloc(request, requestSize);
        sprintf(strchr(request, '\0'), "\r\n");
    }

    return request;
}

