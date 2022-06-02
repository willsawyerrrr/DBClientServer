/*
 * dbserver.c
 *      CSSE2310 - Assignment Three - Semester One, 2022
 *
 *      Written by William Sawyer, w.sawyer@uqconnect.edu.au
 *
 * Usage:
 *      dbserver authfile connections [portnum]
 */

#include "address.h"
#include "dbserver.h"

#include <csse2310a4.h>
#include <netdb.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stringstore.h>
#include <sys/socket.h>
#include <unistd.h>

#define MIN_ARGS 3
#define MAX_ARGS 4

// macro instead of Enum since more exist where external functions: i.e. those
// in the header 'address.h'; may cause the process to exit.
#define EXIT_INVALID_COMMAND 1
#define EXIT_CANNOT_AUTHENTICATE 2
#define EXIT_CANNOT_LISTEN 3

#define AUTHFILE_ARG 1
#define CONNECTIONS_ARG 2
#define PORTNUM_ARG 3

// size required to store status explanations - including '\0'
#define OK_LENGTH 3
#define NOT_FOUND_LENGTH 10
#define BAD_REQ_LENGTH 12

// size required to store the name and value of the Content-Length header
#define CON_LEN_SIZE 15
#define CON_VAL_SIZE 3

int main(int argc, char* argv[]) {
    validate_arguments(argc, argv);

    StringStore* public = stringstore_init();
    sem_t publicLock;
    sem_init(&publicLock, 0, 1);

    StringStore* private = stringstore_init();
    sem_t privateLock;
    sem_init(&privateLock, 0, 1);

    FILE* authfile = get_authfile(argv[AUTHFILE_ARG]);

    int connections = atoi(argv[CONNECTIONS_ARG]);
    char* port = argv[PORTNUM_ARG] ? argv[PORTNUM_ARG] : "0";
    int server = begin_listening(port, connections);

    int portnum = get_portnum(server);
    fprintf(stderr, "%d\n", portnum);
    fflush(stderr);

    process_connections(server, public, publicLock, private, privateLock);

    stringstore_free(public);
    sem_destroy(&publicLock);

    stringstore_free(private);
    sem_destroy(&privateLock);
    return EXIT_SUCCESS;
}

void validate_arguments(int argc, char* argv[]) {
    if (argc < MIN_ARGS || argc > MAX_ARGS
            || !validate_connections(argv[2])
            || (argc > 3 && !validate_portnum(argv[3]))) {
        fprintf(stderr, "Usage: dbserver authfile connections [portnum]\n");
        fflush(stderr);
        exit(EXIT_INVALID_COMMAND);
    }
}

bool validate_connections(char* arg) {
    int connections = atoi(arg);
    return (validate_integral_arg(arg) && connections >= 0);
}

bool validate_portnum(char* arg) {
    int portnum = atoi(arg);
    return (validate_integral_arg(arg)
            && (!portnum || (portnum >= 1024 && portnum <= 65535)));
}

bool validate_integral_arg(char* arg) {
    for (int i = 0; arg[i]; i++) {
        if (!isdigit(arg[i])) {
            return false;
        }
    }

    return strlen(arg); // only return true if arg is non-empty
}

FILE* get_authfile(char* filename) {
    FILE* authfile = fopen(filename, "r");

    if (!authfile) {
        fprintf(stderr, "dbserver: unable to read authentication string\n");
        fflush(stderr);
        exit(EXIT_CANNOT_AUTHENTICATE);
    }

    return authfile;
}

int begin_listening(char* port, int connections) {
    struct sockaddr* address = get_addr(port);
    int server = socket(AF_INET, SOCK_STREAM, 0);

    int set = 1;
    if (!address
            || setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int))
            || bind(server, address, sizeof(struct sockaddr))
            || listen(server, connections)) {
        // error trying to get address info, set socket options, bind or listen
        fprintf(stderr, "dbserver: unable to open socket for listening\n");
        fflush(stderr);
        exit(EXIT_CANNOT_LISTEN);
    }

    return server;
}

int get_portnum(int server) {
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    socklen_t size = sizeof(address);
    if (getsockname(server, (struct sockaddr*) &address, &size)) {
        return 0;
    }

    return htons(address.sin_port); // convert to network-byte order
}

void process_connections(int server, StringStore* public, sem_t publicLock,
        StringStore* private, sem_t privateLock) {
    while (1) {
        ThreadArgs* ta = malloc(sizeof(ThreadArgs));
        memset(ta, 0, sizeof(ThreadArgs));
        ta->public = public;
        ta->publicLock = publicLock;
        ta->private = private;
        ta->privateLock = privateLock;

        ta->socket = accept(server, 0, 0);

        pthread_t threadId;
        pthread_create(&threadId, NULL, client_thread, (void*) ta);
        pthread_detach(threadId);
    }

    return;
}

void* client_thread(void* arg) {
    ThreadArgs* ta = (ThreadArgs*) arg;

    StringStore* public = ta->public;
    sem_t publicLock = ta->publicLock;

    StringStore* private = ta->private;
    sem_t privateLock = ta->privateLock;

    int socket = ta->socket;
    free(ta);

    int socket2 = dup(socket);
    FILE* read = fdopen(socket, "r");
    FILE* write = fdopen(socket2, "w");

    while (true) {
        // get request
        char* method;
        char* address;
        HttpHeader** reqHeaders;
        char* body;

        int valid = get_HTTP_request(read, &method, &address, &reqHeaders,
                &body);

        if (!valid) {
            break;
        }

        // split address into database and key
        char** splitAddress = split_by_char(address, '/', 3);
        char* dbName = splitAddress[1]; // skip over empty string at start
        char* key = splitAddress[2];

        StringStore* database = NULL;
        sem_t lock;
        if (!strncmp(dbName, "public", 7)) {
            database = public;
            lock = publicLock;
        } else if (!strncmp(dbName, "private", 8)) {
            database = private;
            lock = privateLock;
        }

        ResponseArgs* ra = get_response_args(method, database, lock, key, body);

        HttpHeader** headers = malloc(sizeof(HttpHeader*));
        memset(headers, 0, sizeof(HttpHeader*));

        HttpHeader* header = malloc(sizeof(HttpHeader));
        memset(header, 0, sizeof(HttpHeader));
        header->name = malloc(CON_LEN_SIZE);
        sprintf(header->name, "Content-Length");
        header->value = malloc(CON_VAL_SIZE);
        sprintf(header->value, "%ld", ra->result ? strlen(ra->result) : 0);
        headers[0] = header;
        char* response = construct_HTTP_response(ra->status,
                ra->statusExplanation, headers, ra->result);
        fprintf(write, "%s", response);
        fflush(write);
        
        free(method);
        free(address);
        free_array_of_headers(reqHeaders);
        free(body);

        free(ra->statusExplanation);
        if (ra->result) {
            free(ra->result);
        }
        free(ra);

        free_array_of_headers(headers); // frees header and its members, too

        free(response);
    }

    // cleanup - everything else freed within loop
    fclose(read);
    fclose(write);
    
    return NULL;
}

ResponseArgs* get_response_args(char* method, StringStore* database,
        sem_t lock, char* key, char* value) {
    int status = 0;;
    char* statusExplanation = NULL;
    char* result = NULL;

    if (!database) {
        status = 400;
    } else if (!strncmp(method, "GET", 4)) {
        sem_wait(&lock);
        if ((result = stringstore_retrieve(database, key))) {
            status = 200;
        } else {
            status = 404;
        }
        sem_post(&lock);
    } else if (!strncmp(method, "PUT", 4)) {
        sem_wait(&lock);
        if (stringstore_add(database, key, value)) {
            status = 200;
        } else {
            status = 404;
        }
        sem_post(&lock);
    } else if (!strncmp(method, "DELETE", 7)) {
        sem_wait(&lock);
        if (stringstore_add(database, key, value)) {
            status = 200;
        } else {
            status = 404;
        }
        sem_post(&lock);
    } else {
        status = 400;
    }

    switch (status) {
        case 200:
            statusExplanation = malloc(OK_LENGTH);
            strncpy(statusExplanation, "OK", OK_LENGTH);
            break;
        case 400:
            statusExplanation = malloc(BAD_REQ_LENGTH);
            strncpy(statusExplanation, "Bad Request", BAD_REQ_LENGTH);
            break;
        case 404:
            statusExplanation = malloc(NOT_FOUND_LENGTH);
            strncpy(statusExplanation, "Not Found", NOT_FOUND_LENGTH);
            break;
        default:
            break;
    }
    
    ResponseArgs* ra = malloc(sizeof(ResponseArgs));
    memset(ra, 0, sizeof(ResponseArgs));
    ra->status = status;
    ra->statusExplanation = statusExplanation;
    ra->result = result;

    return ra;
}

