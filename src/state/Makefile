C = g++
CCFLAGS = -lGL -lglut -lm -DGL_GLEXT_PROTOTYPES -o5
CCDEBUGFLAGS = -lGL -lglut -g -lm -DGL_GLEXT_PROTOTYPES
CCFLAGSBOOSt = -lboost_regex-mt
OUTNAME = gamestatedriver
SRC_DIR = .
INCL_DIR = include
FILES = ./*.cpp

build: ${FILES}
	${C} ${CCFLAGS} ${SRC_DIR}/*.cpp -o ${OUTNAME} 

debug: ${FILES}
	${C} ${CCDEBUGFLAGS} ${SRC_DIR}/*.cpp -o ${OUTNAME} 

