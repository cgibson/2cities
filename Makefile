CLINK = g++ -o release
C = g++ -c
CCFLAGS = -lGL -lGLU -lglut -lm -DGL_GLEXT_PROTOTYPES
CCDEBUGFLAGS = -lGL -lGLU -lglut -g -lm -DGL_GLEXT_PROTOTYPES

SRC_DIR = .
FILES = ./*.cpp

build: global.o StateManager.o graphics.o renderer.o Vector.o src/system/main.cpp
	${CLINK} ${CCFLAGS} src/system/main.cpp StateManager.o renderer.o Vector.o graphics.o global.o
	rm *.o

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

clean:
	rm *.o release
