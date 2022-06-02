#include <stdio.h>
#include <stdlib.h>

/* construct_HTTP_request()
 * ------------------------
 * Returns a malloc()-allocated buffer containing the HTTP request appropriate
 * for the given action, database, key and value.
 *
 * action: what this request is trying to do
 * database: which database to interact with
 * key: key to query or write to
 * value: if present, value to write to given key
 *
 * It is the caller's responsibility to free this buffer.
 */
char* construct_HTTP_request(char* action, char* database, char* key,
        char* value);
