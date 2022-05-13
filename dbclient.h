#ifndef DBCLIENT_H
#define DBCLIENT_H

#include <stdbool.h>

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
 * are invalid, the process exits accordingly.
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

#endif

