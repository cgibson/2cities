#ifndef LIGHTING_INC
#define LIGHTING_INC

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdio.h>

#include "../helper/Vector.h"
#include "shader.h"

#define MAX_LIGHTS 3
 
class LightData {
  public:
    GLfloat position[3];
    GLfloat diffuse[4];
    GLfloat specular[4];
    

    LightData();
    virtual ~LightData();
};

class Lighting {
  public:
    GLfloat position[3];
    GLfloat diffuse[4];
    GLfloat ambient[4];
    void doLighting(Shader sh, const char *uniform);
    void setLightPos(GLfloat x, GLfloat y, GLfloat z);
    void createLight(GLfloat x, GLfloat y, GLfloat z,
 		           GLfloat Dr, GLfloat Dg, GLfloat Db,
 		           GLfloat Ar, GLfloat Ag, GLfloat Ab);
};

#endif
