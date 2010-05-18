#ifndef MATERIAL_INC
#define MATERIAL_INC

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdio.h>

class Material {
  public:
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
    
    Material(GLfloat in_ambient[4], GLfloat in_diffuse[4], GLfloat in_specular[4], GLfloat in_shininess[1]);
    
    void applyMaterial(GLint program, const char *uniform);
    
    void applyMaterial(int program, int location);
    
    void interpolateFloats(GLfloat *list1, GLfloat *list2, 
                           GLfloat **ret, int count, float t);
    
    Material(){};
private:
};

#endif
