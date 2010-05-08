C = g++

CCFLAGS = -g -DGL_GLEXT_PROTOTYPES
LDFLAGS = -lm
#OUTNAME = network

ROOT = ../../../..
INCLUDE = ${ROOT}/include/

build: client server

client: client.o
	${C} ${LDFLAGS} client.o -o client

client.o: client.cpp
	${C} -c ${CCFLAGS} -I${INCLUDE} client.cpp

server: server.o
	${C} ${LDFLAGS} server.o -o server

server.o: server.cpp
	${C} -c ${CCFLAGS} -I${INCLUDE} server.cpp

clean:
	rm client server *.o
