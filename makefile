CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -I/local/courses/csse2310/include -pthread
LDFLAGS = -L/local/courses/csse2310/lib
LDLIBS = -lcsse2310a3 -lcsse2310a4 -lstringstore -pthread -lm

EXECS = dbclient dbserver
OBJS = address.o dbclient.o dbserver.o request.o stringstore.o
SHARED = libstringstore.so

.PHONY = all clean
.DEFAULT_GOAL := all

all: ${EXECS} ${SHARED}

${EXECS}: %: %.o address.o request.o

${OBJS}: %.o: %.c %.h

libstringstore.so: stringstore.o
	${CC} -shared -o $@ stringstore.o

clean:
	@rm -f ${SHARED} ${OBJS} ${EXECS} testfiles
