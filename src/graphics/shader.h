#ifndef _SHADER_H_
#define _SHADER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "../helper/FBOHelper.h"

using namespace std;

namespace shader{
  
  extern GLint current_shader;
  
  void reset();
  void setProgram(GLint program);
  
};

class Shader {
public:
  Shader();
  virtual void enable();
  GLint getProgram();
protected:
  int compile(const GLchar *vert, const GLchar *frag);
  
  GLint shader_program;
  //bool enabled;
  //GLuint tmp_program;
};

class DefaultShader : public Shader {
public:
  int load(string file);
};

class PassShader : public DefaultShader {
public:
  void setFBO(FBOHelper fbo);
  void setTextures(GLuint *tex, int count);
  void update();
  void enable();
  void disable();
protected:
  GLuint* textures;
  FBOHelper assignedFBO;
  bool useFBO;
};


#endif
