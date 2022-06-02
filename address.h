#ifndef ADDRESS_H
#define ADDRESS_H

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

/* get_addr()
 * ---------
 * Returns an internet-specific socket address structure corresponding to the
 * given port on localhost.
 *
 * port: string representation of the port to get an internet address for
 *
 * Returns the appropriate address if the port is valid; otherwise returns
 * NULL.
 */
struct sockaddr* get_addr(char* port);

#endif

