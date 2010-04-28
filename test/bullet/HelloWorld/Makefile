C = g++

CCFLAGS = -g -DGL_GLEXT_PROTOTYPES
LDFLAGS = -lGL -lGLU -lglut -lm
OUTNAME = helloWorld

ROOT = ../../..
BULLET_LIB = ${ROOT}/lib/linux/
BULLET_INCLUDE = ${ROOT}/include/bullet/
BULLET_OBJS = ${BULLET_LIB}libBulletDynamics.a ${BULLET_LIB}libBulletCollision.a ${BULLET_LIB}libLinearMath.a

build: HelloWorld.o
	${C} ${LDFLAGS} HelloWorld.o ${BULLET_OBJS} -o ${OUTNAME} 

HelloWorld.o: HelloWorld.cpp
	${C} -c ${CCFLAGS} -I${BULLET_INCLUDE} HelloWorld.cpp
