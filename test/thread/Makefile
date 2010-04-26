C = g++
CCFLAGS = -lm -lboost_thread-mt
CCDEBUGFLAGS = -g -lm -lboost_thread-mt
OUTNAME = release
SRC_DIR = .
INCL_DIR = src/include
FILES = ./*.cpp

test-thread: ${FILES}
	${C} ${CCFLAGS} threadTest.cpp -o ${OUTNAME} 


