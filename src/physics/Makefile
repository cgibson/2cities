C = g++

CCFLAGS =  -lGL -lGLU -lglut -lm -g -DGL_GLEXT_PROTOTYPES
OUTNAME = Physics

ROOT = ../..
BULLET_LIB = ${ROOT}/lib/bullet

LIB = -I${BULLET_LIB}

BULLET_LINK = ${BULLET_LIB}/BulletDynamics/libBulletDynamics.a ${BULLET_LIB}/BulletCollision/libBulletCollision.a ${BULLET_LIB}/LinearMath/libLinearMath.a

all: Physics.o

Physics.o: Physics.cpp
	${C} -c ${CCFLAGS} ${LIB} Physics.cpp
