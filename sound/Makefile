#Generalized, self-updating Makefile
# Author: Harrison McKenzie Chapter (HMC)

CXX=g++
CXXFLAGS=-O3 -Wall
LDFLAGS=-Llib -lGL -lGLU -lglut -lSDL -lSDL_mixer -lm
OBJS= $(shell for i in *.cpp; do echo `basename $$i .cpp`.o; done)
PROG=program

all: depend $(OBJS)
	$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) lib/libSDL_mixer.a

depend:
	gccmakedep *.cpp &> /dev/null

clean:
	rm *.o $(PROG) Makefile.bak
# DO NOT DELETE
