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
    GLfloat data[16];
    
    Material(GLfloat in_ambient[4], GLfloat in_diffuse[4], GLfloat in_specular[4], GLfloat in_shininess[1])
    {
      int i;
      for(i = 0; i < 4; i++)
      {
        data[i + (4 * 0)] = in_ambient[i];
        data[i + (4 * 1)] = in_diffuse[i];
        data[i + (4 * 2)] = in_specular[i];
      }
      data[12] = in_shininess[0];
    }
    
    void applyMaterial(GLint program, const char *uniform)
    {
      int loc = glGetUniformLocation(program, uniform);
      applyMaterial(program, loc);
    }
    
    void applyMaterial(int program, int location) {
      glUniform4fv(location, 4, const_cast<GLfloat*>(data));
    }
    
    void interpolateFloats(GLfloat *list1, GLfloat *list2, 
                           GLfloat **ret, int count, float t)
    {
      float diff;
      for(int i = 0; i < count; i++)
      {
        diff = list2[i] - list1[i];
        (*ret)[i] = list1[i] + diff * t;
      }
    }
    
    /*Material interpolate(Material other, float t)
    {
      Material res;
      GLfloat *rAmbient = (GLfloat *)(res.ambient);
      GLfloat *rDiffuse = (GLfloat *)(res.diffuse);
      GLfloat *rSpecular = (GLfloat *)(res.specular);
      GLfloat *rShininess = (GLfloat *)(res.shininess);
      interpolateFloats((GLfloat*)ambient, (GLfloat*)other.ambient, 
                        (GLfloat**)&rAmbient, 4, t);
      interpolateFloats((GLfloat*)diffuse, (GLfloat*)other.diffuse, 
                        (GLfloat**)&rDiffuse, 4, t);
      interpolateFloats((GLfloat*)specular, (GLfloat*)other.specular, 
                        (GLfloat**)&rSpecular, 4, t);
      interpolateFloats((GLfloat*)shininess, (GLfloat*)other.shininess, 
                        (GLfloat**)&rShininess, 1, t);
      
      return res;
    }*/
    Material(){};
private:
};

#endif
