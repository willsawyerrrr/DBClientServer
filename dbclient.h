#ifndef DBCLIENT_H
#define DBCLIENT_H

#include <stdbool.h>

/* validate_num_arguments()
 * ------------------------
 * Determines whether the given number of arguments is valid. The number of
 * arguments if and only if it is greater than 3, implying all required
 * arguments are given, as follows:
 *     dbclient portnum key [value]
 *
 * Return: true if the given number of arguments is valid; false otherwise.
 */
bool validate_num_arguments(int argc);

/* validate_key()
 * --------------
 * Determines whether the given key is valid. The key is valid if and only if
 * it does not contain spaces or newline characters.
 *
 * Return: true if the given key is valid; false otherwise.
 */
bool validate_key(char* key);

#endif
