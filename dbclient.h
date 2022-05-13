#ifndef DBCLIENT_H
#define DBCLIENT_H

#include <stdbool.h>

/* validate_arguments()
 * --------------------
 * Determines whether the given arguments are valid. The arguments are valid if
 * and only if the given number of arguments is sufficient and the key argument
 * is valid - see validate_key().
 *
 * Returns if and only if the given aruments are valid. If the given arguments
 * are invalid, the program exits accordingly.
 */
void validate_arguments(int argc, char* argv[]);

/* validate_key()
 * --------------
 * Determines whether the given key is valid. The key is valid if and only if
 * it does not contain spaces or newline characters.
 *
 * Returns true if and only if the given key is valid; false otherwise.
 */
bool validate_key(char* key);

#endif
