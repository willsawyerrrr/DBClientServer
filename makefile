CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -I /local/courses/csse2310/include
LDFLAGS = -L /local/courses/csse2310/lib
LDLIBS = -l csse2310a3 -l csse2310a4

EXECS = dbclient dbserver
OBJS = dbclient.o dbserver.o stringstore.o
SHOBS = stringstore.so

.PHONY = all clean
.DEFAULT_GOAL := all

all: ${EXECS}

${EXECS}: %: %.o

${OBJS}: %.o: %.c %.h

${SHOBS}: %.so: %.o

clean:
	@rm -f ${OBJS} ${EXECS} testfiles
