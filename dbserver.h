#ifndef DBSERVER_H
#define DBSERVER_H

#include <stdbool.h>
#include <stdio.h>
#include <stringstore.h>

typedef struct {
    StringStore* public;
    StringStore* private;
    int socket;
} ThreadArgs;

typedef struct {
    int status;
    char* statusExplanation;
    char* result;
} ResponseArgs;

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

/* begin_listening()
 * -------------
 * Returns a file descriptor referring to the socket used to communicate over
 * the specified port (or ephemeral port if the specified port is 0).
 *
 * port: string representation of the port to connect to (or 0)
 *
 * If an error occurs while setting up a socket to listen, the process emits
 * an error message and exits with status 3.
 */
int begin_listening(char* port, int connections);

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

/* process_connections()
 * ---------------------
 * Spawns client-handling threads to interact with incoming connections.
 *
 * server: file descriptor referring to the server's communication socket
 *
 * Does not return any value.
 */
void process_connections(int server, StringStore* public,
        StringStore* private);

/* client_thread()
 * ---------------
 * Handles communication over specific connection with a client.
 *
 * arg: void* cast of ThreadArgs* object containing the communication socket
 *      and databases
 *
 * Returns NULL.
 */
void* client_thread(void* arg);

/* get_response_args()
 * -------------------
 * Processes the request summarised by the given method, database, key and
 * value.
 *
 * method: action to take on the database
 * database: database to act on
 * key: key to act on within the database
 * value: if not NULL, value to assign to key within the database
 *
 * Returns a structure of arguments for constructing a HTTP response.
 */
ResponseArgs* get_response_args(char* method, StringStore* database,
        char* key, char* value);

#endif
