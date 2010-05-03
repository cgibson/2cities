# which c compiler to use
CC = g++

# output names of the executables
LINUX_BIN = 2cities
WIN32_BIN = 2cities.exe

# directories to include header files for libraries
# general format is -Iinclude/<library directory>
INCLUDE_DIRS = -Iinclude/bullet -Iinclude/freetype

# anything that needs to be defined for libraries or our code
DEFINES = -DGL_GLEXT_PROTOTYPES -DOGLFT_NO_SOLID -DOGLFT_NO_QT

# directory that holds our static libraries
LINUX_LIBDIR = lib/linux
WIN32_LIBDIR = lib/win32

# static libraries we're linking with
LINUX_LIBS = $(LINUX_LIBDIR)/libBulletDynamics.a \
		   	 $(LINUX_LIBDIR)/libBulletCollision.a \
		   	 $(LINUX_LIBDIR)/libLinearMath.a \
		   	 $(LINUX_LIBDIR)/libfreetype.a \
			 $(LINUX_LIBDIR)/libz.a
WIN32_LIBS = $(WIN32_LIBDIR)/libBulletDynamics.a \
             $(WIN32_LIBDIR)/libBulletCollision.a \
             $(WIN32_LIBDIR)/libLinearMath.a \
             $(WIN32_LIBDIR)/libfreetype.a \
             $(WIN32_LIBDIR)/libz.a

# compiler (NOT LINKER!) flags, such as the optimizer, include paths, defines, etc.
CCFLAGS_DEBUG = -g $(INCLUDE_DIRS) $(DEFINES)
CCFLAGS_RELEASE = -O3 $(INCLUDE_DIRS) $(DEFINES)

# are we compiling in debug or release mode?
CCFLAGS = $(CCFLAGS_DEBUG)

# linker (NOT COMPILER!) flags, such as which libraries to link with
LDFLAGS = -lGL -lGLU -lglut -lm

# list of all object files
OBJS = global.o io.o NetworkClient.o NetworkServer.o PhysObj.o \
	   BuildState.o CarnageState.o InGameState.o StateManager.o \
	   graphics.o renderer.o hud.o console.o Vector.o main.o \
	   Physics.o GLSL_helper.o Lighting.o FBOHelper.o OGLFT.o

# default build rule (linux)
all: linux

# linux build rule
linux: startlinux svnrev $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LINUX_LIBS) -o $(LINUX_BIN)
	@echo "========== BUILD COMPLETE [linux] =========="

startlinux:
	@echo "========== BUILD STARTING [linux] =========="

win32: startwin32 svnrev $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(WIN32_LIBS) -o $(WIN32_BIN)
	@echo "========== BUILD COMPLETE [win32] =========="

startwin32:
	@echo "========== BUILD STARTING [win32] =========="

# build clean up
clean: startclean
	-rm *.o
	-rm svnrev.h 
	-rm $(LINUX_BIN)
	-rm $(WIN32_BIN)
	@echo "========== CLEAN COMPLETE =========="

startclean:
	@echo "========== CLEAN STARTING =========="

# fancy build rule to generate a .h file with the current svn rev number in it
svnrev:
	echo "#ifndef _SVNREV_H_" > svnrev.h
	echo "#define _SVNREV_H_" >> svnrev.h
	echo "#define SVN_REV \"`svnversion`\"" >> svnrev.h
	echo "#endif" >> svnrev.h

# individual object file build rules
main.o: src/system/main.cpp
	$(CC) -c $(CCFLAGS) src/system/main.cpp

Physics.o: src/physics/Physics.cpp
	$(CC) -c $(CCFLAGS) src/physics/Physics.cpp

NetworkClient.o: src/network/NetworkClient.cpp
	$(CC) -c $(CCFLAGS) src/network/NetworkClient.cpp

NetworkServer.o: src/network/NetworkServer.cpp
	$(CC) -c $(CCFLAGS) src/network/NetworkServer.cpp

PhysObj.o: src/state/PhysObj.cpp
	$(CC) -c $(CCFLAGS) src/state/PhysObj.cpp

InGameState.o: src/state/InGameState.cpp
	$(CC) -c $(CCFLAGS) src/state/InGameState.cpp

CarnageState.o: src/state/CarnageState.cpp
	$(CC) -c $(CCFLAGS) src/state/CarnageState.cpp

BuildState.o: src/state/BuildState.cpp
	$(CC) -c $(CCFLAGS) src/state/BuildState.cpp

StateManager.o: src/state/StateManager.cpp
	$(CC) -c $(CCFLAGS) src/state/StateManager.cpp
	
renderer.o: src/graphics/renderer.cpp
	$(CC) -c $(CCFLAGS) src/graphics/renderer.cpp
	
Vector.o: src/helper/Vector.cpp
	$(CC) -c $(CCFLAGS) src/helper/Vector.cpp
	
graphics.o: src/graphics/graphics.cpp
	$(CC) -c $(CCFLAGS) src/graphics/graphics.cpp
	
global.o: src/system/global.cpp
	$(CC) -c $(CCFLAGS) src/system/global.cpp
	
io.o: src/system/io.cpp
	$(CC) -c $(CCFLAGS) src/system/io.cpp

hud.o: src/graphics/hud.cpp
	$(CC) -c $(CCFLAGS) src/graphics/hud.cpp

console.o: src/graphics/console.cpp
	$(CC) -c $(CCFLAGS) src/graphics/console.cpp

GLSL_helper.o: src/helper/GLSL_helper.cpp
	$(CC) -c $(CCFLAGS) src/helper/GLSL_helper.cpp

FBOHelper.o: src/helper/FBOHelper.cpp
	$(CC) -c $(CCFLAGS) src/helper/FBOHelper.cpp
	
Lighting.o: src/graphics/Lighting.cpp
	$(CC) -c $(CCFLAGS) src/graphics/Lighting.cpp

OGLFT.o: src/graphics/OGLFT.cpp
	$(CC) -c $(CCFLAGS) src/graphics/OGLFT.cpp
