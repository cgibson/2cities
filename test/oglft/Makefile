C = g++

CCFLAGS = -O2 -DGL_GLEXT_PROTOTYPES -DOGLFT_NO_SOLID -DOGLFT_NO_QT
LDFLAGS = -lGL -lglut -lm -lfreetype
OUTNAME = helloOGLFT

ROOT = ../..
LIBDIR = ${ROOT}/lib/linux
FREETYPE_INCLUDE = ${ROOT}/include/freetype
FREETYPE_OBJS = ${LIBDIR}/libfreetype.a
OGLFT_INCLUDE = ${ROOT}/include/oglft
OGLFT_OBJS = ${LIBDIR}/libOGLFT.a

build: helloOGLFT.o
	${C} ${LDFLAGS} helloOGLFT.o ${FREETYPE_OBJS} ${OGLFT_OBJS} -o ${OUTNAME}

helloOGLFT.o: helloOGLFT.cpp
	${C} -c ${CCFLAGS} -I${FREETYPE_INCLUDE} -I${OGLFT_INCLUDE} helloOGLFT.cpp

clean:
	rm *.o
	rm ${OUTNAME}
