# which c compiler to use
CC = g++

# output names of the executables
CLIENT_BIN = 2cities-client
SERVER_BIN = 2cities-server

# directories to include header files for libraries
# general format is -Iinclude/<library directory>
INCLUDE_DIRS = -Iinclude/bullet -Iinclude/freetype -Iinclude/lua

# anything that needs to be defined for libraries or our code
DEFINES = -DGL_GLEXT_PROTOTYPES -DOGLFT_NO_SOLID -DOGLFT_NO_QT

# directory that holds our static libraries
LIBDIR = lib/linux

# static libraries we're linking with
LIBS = $(LIBDIR)/libBulletDynamics.a \
	   $(LIBDIR)/libBulletCollision.a \
	   $(LIBDIR)/libLinearMath.a \
	   $(LIBDIR)/libfreetype.a \
	   $(LIBDIR)/libz.a \
	   $(LIBDIR)/liblua.a

# compiler (NOT LINKER!) flags, such as the optimizer, include paths, defines, etc.
CCFLAGS_DEBUG = -g $(INCLUDE_DIRS) $(DEFINES)
CCFLAGS_RELEASE = -O3 $(INCLUDE_DIRS) $(DEFINES)

# are we compiling in debug or release mode?
CCFLAGS = $(CCFLAGS_DEBUG)

# linker (NOT COMPILER!) flags, such as which libraries to link with
LDFLAGS = -lGL -lGLU -lglut -lm

# list of all object files
CLIENT_OBJS = global.o io.o Vector.o quaternion.o \
	   BuildState.o CarnageState.o InGameState.o StateManager.o \
	   hud.o console.o graphics.o renderer.o \
	   GLSL_helper.o Lighting.o FBOHelper.o OGLFT.o luahelper.o \
	   Physics.o PhysicsBody.o PhysObj.o WorldObject.o factory.o \
	   UIControl.o GameUI.o BlankUI.o \
	   NetworkSystem.o NetworkPrivate.o NetworkClient.o NetworkServer.o \
	   main.o

# Should be just these items but WorldObject requires graphics items currently
#SERVER_OBJS = global.o io.o Vector.o quaternion.o \
#	   BuildState.o CarnageState.o InGameState.o StateManager.o \
#	   Physics.o PhysicsBody.o PhysObj.o WorldObject.o factory.o \
#	   NetworkSystem.o NetworkPrivate.o NetworkServer.o mainServer.o
	   
SERVER_OBJS = global.o io.o Vector.o quaternion.o \
	   BuildState.o CarnageState.o InGameState.o StateManager.o \
	   hud.o console.o graphics.o renderer.o \
	   GLSL_helper.o Lighting.o FBOHelper.o OGLFT.o luahelper.o \
	   Physics.o PhysicsBody.o PhysObj.o WorldObject.o factory.o \
	   UIControl.o GameUI.o BlankUI.o \
	   NetworkSystem.o NetworkPrivate.o NetworkClient.o NetworkServer.o \
	   mainServer.o

# default build rule (client and server)
all: client server

# client build rule
client: startclient svnrev $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $(CLIENT_OBJS) $(LIBS) -o $(CLIENT_BIN)
	@echo "========== BUILD COMPLETE [client] =========="

startclient:
	@echo "========== BUILD STARTING [client] =========="

server: startserver svnrev $(SERVER_OBJS)
	$(CC) $(LDFLAGS) $(SERVER_OBJS) $(LIBS) -o $(SERVER_BIN)
	@echo "========== BUILD COMPLETE [server] =========="

startserver:
	@echo "========== BUILD STARTING [server] =========="

# build clean up
clean: startclean
	-rm *.o
	-rm svnrev.h 
	-rm $(CLIENT_BIN)
	-rm $(SERVER_BIN)
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
# System	
main.o: src/system/main.cpp
	$(CC) -c $(CCFLAGS) src/system/main.cpp

mainServer.o: src/system/mainServer.cpp
	$(CC) -c $(CCFLAGS) src/system/mainServer.cpp

global.o: src/system/global.cpp
	$(CC) -c $(CCFLAGS) src/system/global.cpp
	
io.o: src/system/io.cpp
	$(CC) -c $(CCFLAGS) src/system/io.cpp

# Math LibrariesNetworkSystem.o
Vector.o: src/helper/Vector.cpp
	$(CC) -c $(CCFLAGS) src/helper/Vector.cpp

quaternion.o: src/math/quaternion.cpp
	$(CC) -c $(CCFLAGS) src/math/quaternion.cpp

# Physics
Physics.o: src/physics/Physics.cpp
	$(CC) -c $(CCFLAGS) src/physics/Physics.cpp

PhysicsBody.o: src/physics/Physics.cpp
	$(CC) -c $(CCFLAGS) src/physics/PhysicsBody.cpp

# Network Items
NetworkSystem.o: src/network/NetworkSystem.cpp
	$(CC) -c $(CCFLAGS) src/network/NetworkSystem.cpp

NetworkPrivate.o: src/network/NetworkPrivate.cpp
	$(CC) -c $(CCFLAGS) src/network/NetworkPrivate.cpp

NetworkClient.o: src/network/NetworkClient.cpp
	$(CC) -c $(CCFLAGS) src/network/NetworkClient.cpp

NetworkServer.o: src/network/NetworkServer.cpp
	$(CC) -c $(CCFLAGS) src/network/NetworkServer.cpp

# Scene Objects
WorldObject.o: src/scene/WorldObject.cpp
	$(CC) -c $(CCFLAGS) src/scene/WorldObject.cpp
	
factory.o: src/scene/factory.cpp
	$(CC) -c $(CCFLAGS) src/scene/factory.cpp

PhysObj.o: src/state/PhysObj.cpp
	$(CC) -c $(CCFLAGS) src/state/PhysObj.cpp
	
# GameStates
InGameState.o: src/state/InGameState.cpp
	$(CC) -c $(CCFLAGS) src/state/InGameState.cpp

CarnageState.o: src/state/CarnageState.cpp
	$(CC) -c $(CCFLAGS) src/state/CarnageState.cpp

BuildState.o: src/state/BuildState.cpp
	$(CC) -c $(CCFLAGS) src/state/BuildState.cpp

StateManager.o: src/state/StateManager.cpp
	$(CC) -c $(CCFLAGS) src/state/StateManager.cpp

# Graphics
renderer.o: src/graphics/renderer.cpp
	$(CC) -c $(CCFLAGS) src/graphics/renderer.cpp

graphics.o: src/graphics/graphics.cpp
	$(CC) -c $(CCFLAGS) src/graphics/graphics.cpp

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

luahelper.o: src/helper/luahelper.cpp
	$(CC) -c $(CCFLAGS) src/helper/luahelper.cpp

UIControl.o: src/graphics/UIControl.cpp
	$(CC) -c $(CCFLAGS) src/graphics/UIControl.cpp

GameUI.o: src/graphics/GameUI.cpp
	$(CC) -c $(CCFLAGS) src/graphics/GameUI.cpp

BlankUI.o: src/graphics/BlankUI.cpp
	$(CC) -c $(CCFLAGS) src/graphics/BlankUI.cpp
