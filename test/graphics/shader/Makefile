C = g++
CCFLAGS = -g -Wall -lGL -lGLU -lglut -lm -DGL_GLEXT_PROTOTYPES -o5
CCDEBUGFLAGS = -lGL -lGLU -lglut -g -lm -DGL_GLEXT_PROTOTYPES
CCFLAGSBOOSt = -lboost_regex-mt
OUTNAME = release
SRC_DIR = src
INCL_DIR = src/include
FILES = src/*.cpp

build: ${FILES}
	${C} ${CCFLAGS} ${SRC_DIR}/*.cpp -o ${OUTNAME} 

debug: ${FILES}
	${C} ${CCDEBUGFLAGS} ${SRC_DIR}/*.cpp -o ${OUTNAME} 

