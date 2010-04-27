C = g++

CCFLAGS =  -lGL -lGLU -lglut -lm -g -DGL_GLEXT_PROTOTYPES
OUTNAME = helloWorld

ROOT = ../../..
BULLET_LIB = ${ROOT}/lib/bullet

LIB = -I${BULLET_LIB}

BULLET_LINK = ${BULLET_LIB}/BulletDynamics/libBulletDynamics.a ${BULLET_LIB}/BulletCollision/libBulletCollision.a ${BULLET_LIB}/LinearMath/libLinearMath.a

build: HelloWorld.o
	${C} ${CCFLAGS} ${LIB} HelloWorld.o ${BULLET_LINK} -o ${OUTNAME} 

HelloWorld.o: HelloWorld.cpp
	${C} -c ${CCFLAGS} ${LIB} HelloWorld.cpp
