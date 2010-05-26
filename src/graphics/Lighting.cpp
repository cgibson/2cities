#include "Lighting.h"
#include "../graphics/graphics.h"

LightData lights[MAX_LIGHTS];

LightData::LightData()
{
 	
}

LightData::~LightData()
{
 	
}

void Lighting::createLight(GLfloat x, GLfloat y, GLfloat z,
	    GLfloat Dr, GLfloat Dg, GLfloat Db,
	    GLfloat Ar, GLfloat Ag, GLfloat Ab)
{
  position[0] = x;
  position[1] = y;
  position[2] = z;
  diffuse[0] = Dr;
  diffuse[1] = Dg;
  diffuse[2] = Db;
  diffuse[3] = 1;
  ambient[0] = Ar;
  ambient[1] = Ag;
  ambient[2] = Ab;
  ambient[3] = 1;
}

void Lighting::setLightPos(GLfloat x, GLfloat y, GLfloat z)
{
  position[0] = x;
  position[1] = y;
  position[2] = z;
}
    
void Lighting::doLighting(Shader sh, const char *uniform)
{
  GLint old = shader::current_shader;
  sh.enable();
  int loc = glGetUniformLocation(sh.getProgram(), "light_position");
  glUniform3fv(loc, 1, position);
  loc = glGetUniformLocation(sh.getProgram(), "light_diffuse");
  glUniform4fv(loc, 1, diffuse);
  loc = glGetUniformLocation(sh.getProgram(), "light_ambient");
  glUniform4fv(loc, 1, ambient);
  shader::setProgram(old);
}

