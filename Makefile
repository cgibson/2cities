CLINK = g++ -o release
C = g++ -c
CCFLAGS = -lGL -lGLU -lglut -lm -DGL_GLEXT_PROTOTYPES
CCDEBUGFLAGS = -lGL -lGLU -lglut -g -lm -DGL_GLEXT_PROTOTYPES

ALL = global.o io.o PhysObj.o CarnageState.o InGameState.o StateManager.o graphics.o renderer.o Vector.o src/system/main.cpp

SRC_DIR = .
FILES = ./*.cpp

build: ${ALL}
	${CLINK} ${CCFLAGS} ${ALL}
	rm *.o

PhysObj.o: src/state/PhysObj.cpp
	${C} ${CCFLAGS} src/state/PhysObj.cpp

InGameState.o: src/state/InGameState.cpp
	${C} ${CCFLAGS} src/state/InGameState.cpp

CarnageState.o: src/state/CarnageState.cpp
	${C} ${CCFLAGS} src/state/CarnageState.cpp

StateManager.o: src/state/StateManager.cpp
	${C} ${CCFLAGS} src/state/StateManager.cpp
	
renderer.o: src/graphics/renderer.cpp
	${C} ${CCFLAGS} src/graphics/renderer.cpp
	
Vector.o: src/helper/Vector.cpp
	${C} ${CCFLAGS} src/helper/Vector.cpp
	
graphics.o: src/graphics/graphics.cpp
	${C} ${CCFLAGS} src/graphics/graphics.cpp
	
global.o: src/system/global.cpp
	${C} ${CCFLAGS} src/system/global.cpp
	
io.o: src/system/io.cpp
	${C} ${CCFLAGS} src/system/io.cpp

clean:
	rm *.o release
