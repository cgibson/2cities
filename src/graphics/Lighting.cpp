#include "Lighting.h"

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
    
void Lighting::doLighting(GLint program, const char *uniform)
{
  int loc = glGetUniformLocation(program, "light.position");
  glUniform3fv(loc, 1, const_cast<GLfloat*>(position));
  loc = glGetUniformLocation(program, "light.diffuse");
  glUniform4fv(loc, 1, const_cast<GLfloat*>(diffuse));
  loc = glGetUniformLocation(program, "light.ambient");
  glUniform4fv(loc, 1, const_cast<GLfloat*>(ambient));
}
    
void Lighting::doLighting(GLint program, int location) {

  //glUniform3fv(location, 3, const_cast<GLfloat*>(data));
}

