#ifndef DBSERVER_H
#define DBSERVER_H

#include <stdbool.h>
#include <stdio.h>

/* validate_arguments()
 * --------------------
 * Determines whether the given arguments are valid. The arguments are valid if
 * and only if the given number of arguments is sufficient and both the
 * connections and portnum arguments are valid - see vaildate_connections() and
 * validate_portnum().
 *
 * argc: number of command line arguments given
 * argv: array of command line arguments
 *
 * Returns if and only if the given arguments are valid. If the given arguments
 * are invalid, the process exits accordingly.
 */
void validate_arguments(int argc, char* argv[]);

/* validate_connections()
 * ----------------------
 * Determines whether the given connections argument is valid. The argument is
 * valid if and only if it is represents a non-negative integer.
 *
 * arg: connections argument to validate
 *
 * Returns true if and only if the given connections argument is valid; false
 * otherwise.
 */
bool validate_connections(char* arg);

/* validate_portnum()
 * ------------------
 * Determines whether the given portnum argument is valid. The argument is
 * valid if and only if it is represents a non-negative integer which is either
 * equal to 0 or within the range 1024 to 65535, inclusive.
 *
 * arg: portnum argument to validate
 *
 * Returns true if and only if the given portnum argument is valid; false
 * otherwise.

 */
bool validate_portnum(char* arg);

/* validate_integral_arg()
 * ------------------------
 * Determines whether the given argument represents a valid integer.
 *
 * arg: integral argument to validate
 *
 * Returns true if and only if the given argument represents a valid integer;
 * false otherwise.
 */
bool validate_integral_arg(char* arg);

/* get_authfile()
 * --------------
 * Returns a stream connected to the authentication file with the specified
 * filename.
 *
 * filename: name of authentication file
 *
 * If no such file can be opened for reading, the process emits an error
 * message exits with status 2.
 */
FILE* get_authfile(char* filename);

/* bind_server()
 * -------------
 * Returns a file descriptor referring to the socket used to communicate over
 * the specified port (or ephemeral port if the specified port is 0).
 *
 * port: string representation of the port to connect to (or 0)
 *
 * If the given port cannot be connected to, the process emits an error message
 * and exits with status 3.
 */
int bind_server(char* port);

/* get_portnum()
 * -------------
 * Gets the port number the given socket is listening on.
 *
 * server: file descriptor for socket
 *
 * Returns the port number the given socket is listening on; returns 0 in the
 * case of an error.
 */
int get_portnum(int server);

#endif
