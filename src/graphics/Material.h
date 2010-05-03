#ifndef MATERIAL_INC
#define MATERIAL_INC

#ifdef _WIN32
    #include <windows.h>
    #include <GL/glew.h>
#endif
#include <GL/glut.h>
#include <stdio.h>

class Material {
  public:
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
    Material(GLfloat in_ambient[4], GLfloat in_diffuse[4], GLfloat in_specular[4], GLfloat in_shininess[1])
    {
      int i;
      for(i = 0; i < 4; i++)
      {
      ambient[i] = in_ambient[i];
      diffuse[i] = in_diffuse[i];
      specular[i] = in_specular[i];
      }
      shininess[0] = in_shininess[0];
    }
    void applyMaterial() {
      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
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
    
    Material interpolate(Material other, float t)
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
    }
    Material(){};
private:
};

#endif
