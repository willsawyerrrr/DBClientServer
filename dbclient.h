#ifndef DBCLIENT_H
#define DBCLIENT_H

#include <stdio.h>

/* validate_arguments()
 * --------------------
 * Determines whether the given arguments are valid. The arguments are valid if
 * and only if the given number of arguments is sufficient and the key argument
 * is valid - see validate_key().
 *
 * argc: number of command line arguments given
 * argv: array of command line arguments
 *
 * Returns if and only if the given arguments are valid. If the given arguments
 * are invalid, the process emits an error message and exits with status 1.
 */
void validate_arguments(int argc, char* argv[]);

/* validate_key()
 * --------------
 * Determines whether the given key argument is valid. The argument is valid if
 * and only if it does not contain spaces or newline characters.
 *
 * key: key argument to validate
 *
 * Returns true if and only if the given key argument is valid; false
 * otherwise.
 */
void validate_key(char* key);

/* establish_connection()
 * ----------------------
 * Attempts to establish a connection to the specified port on localhost and
 * returns a file descriptor referring to the socket used to communicate over
 * this connection.
 *
 * port: string representation of port to connect to
 *
 * If the specified port does not result in a successful connection, the
 * process emits an error message and exits with status 2.
 */
int establish_connection(char* port);

/* handle_response()
 * -----------------
 * Parses the HTTP response sent to the given stream and handles the response
 * according to the action requested.
 *
 * stream: stream connecting this client process to the server
 * action: whether this client's request was to GET or PUT
 *
 * Returns the appropriate process exit code to be used after cleanup.
 */
int handle_response(FILE* stream, char* action);

#endif

