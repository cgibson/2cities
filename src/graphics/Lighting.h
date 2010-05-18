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

#define MAX_LIGHTS 3
 
class LightData {
  public:
    GLfloat data[9]; // 3->pos, 3->diffuse, 3->ambient
    

    LightData();
    virtual ~LightData();
};

class Lighting {
  public:
    GLfloat data[9]; // 3->pos, 3->diffuse, 3->ambient
    void doLighting(GLint program,int location);
    void doLighting(GLint program, const char *uniform);
    void setLightPos(GLfloat x, GLfloat y, GLfloat z);
    void createLight(GLfloat x, GLfloat y, GLfloat z,
 		           GLfloat Dr, GLfloat Dg, GLfloat Db,
 		           GLfloat Ar, GLfloat Ag, GLfloat Ab);
};

#endif
