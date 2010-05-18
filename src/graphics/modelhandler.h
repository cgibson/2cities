#ifndef _MODEL_HANDLER_H_
#define _MODEL_HANDLER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>

class ModelHandler { // AKA Badass class
public:
  ModelHandler();
  void drawShadedCube(float size);
  void initialize();
private:
  GLuint genShadedCubeDL();
  GLuint shadedCubeList;

};

#endif
