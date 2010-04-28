# which c compiler to use
CC = g++

# output name of the executable
OUT = release

# directories to include header files for libraries
# general format is -Iinclude/<library directory>
INCLUDE_DIRS = -Iinclude/bullet -Iinclude/freetype -Iinclude/oglft

# anything that needs to be defined for libraries or our code
DEFINES = -DGL_GLEXT_PROTOTYPES -DOGLFT_NO_SOLID -DOGLFT_NO_QT

# directory that holds our static libraries
LIB_DIR = lib/linux

# static libraries we're linking with
LIB_OBJS = ${LIB_DIR}/libBulletDynamics.a ${LIB_DIR}/libBulletCollision.a ${LIB_DIR}/libLinearMath.a ${LIB_DIR}/libfreetype.a ${LIB_DIR}/libOGLFT.a

# compiler (NOT LINKER!) flags, such as the optimizer, include paths, defines, etc.
CCFLAGS = -g ${INCLUDE_DIRS} ${DEFINES}

# linker (NOT COMPILER!) flags, such as which libraries to link with
LDFLAGS = -lGL -lglut -lm -lfreetype

# list of all object files
OBJS = global.o io.o PhysObj.o CarnageState.o InGameState.o StateManager.o graphics.o renderer.o hud.o console.o Vector.o main.o

# main build rule (also deletes object files... we prolly shouldn't do that...)
build: svnrev ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} ${LIB_OBJS} -o ${OUT}
	rm *.o

# fancy build rule to generate a .h file with the current svn rev number in it
svnrev:
	echo "#ifndef _SVNREV_H_" > svnrev.h
	echo "#define _SVNREV_H_" >> svnrev.h
	echo "#define SVN_REV \"`svnversion`\"" >> svnrev.h
	echo "#endif" >> svnrev.h

main.o: src/system/main.cpp
	${CC} -c ${CCFLAGS} src/system/main.cpp

PhysObj.o: src/state/PhysObj.cpp
	${CC} -c ${CCFLAGS} src/state/PhysObj.cpp

InGameState.o: src/state/InGameState.cpp
	${CC} -c ${CCFLAGS} src/state/InGameState.cpp

CarnageState.o: src/state/CarnageState.cpp
	${CC} -c ${CCFLAGS} src/state/CarnageState.cpp

StateManager.o: src/state/StateManager.cpp
	${CC} -c ${CCFLAGS} src/state/StateManager.cpp
	
renderer.o: src/graphics/renderer.cpp
	${CC} -c ${CCFLAGS} src/graphics/renderer.cpp
	
Vector.o: src/helper/Vector.cpp
	${CC} -c ${CCFLAGS} src/helper/Vector.cpp
	
graphics.o: src/graphics/graphics.cpp
	${CC} -c ${CCFLAGS} src/graphics/graphics.cpp
	
global.o: src/system/global.cpp
	${CC} -c ${CCFLAGS} src/system/global.cpp
	
io.o: src/system/io.cpp
	${CC} -c ${CCFLAGS} src/system/io.cpp

hud.o: src/graphics/hud.cpp
	${CC} -c ${CCFLAGS} src/graphics/hud.cpp

console.o: src/graphics/console.cpp
	${CC} -c ${CCFLAGS} src/graphics/console.cpp

clean:
	rm *.o svnrev.h ${OUT}
