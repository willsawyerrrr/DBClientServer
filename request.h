#include <stdio.h>
#include <stdlib.h>

/*
 * Returns a malloc()-allocated buffer containing the HTTP request appropriate
 * for the given action, database, key and value.
 *
 * It is the caller's responsibility to free this buffer.
 */
char* construct_HTTP_request(char* action, char* database, char* key,
        char* value);
