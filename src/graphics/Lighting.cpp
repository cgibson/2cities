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
  data[0] = x;
  data[1] = y;
  data[2] = z;
  data[3] = Dr;
  data[4] = Dg;
  data[5] = Db;
  data[6] = Ar;
  data[7] = Ag;
  data[8] = Ab;
}

void Lighting::setLightPos(GLfloat x, GLfloat y, GLfloat z)
{
  data[0] = x;
  data[1] = y;
  data[2] = z;
}
    
void Lighting::doLighting(GLint program, const char *uniform)
{
  int loc = glGetUniformLocation(program, uniform);
  doLighting(program, loc);
}
    
void Lighting::doLighting(GLint program, int location) {

  glUniform3fv(location, 3, const_cast<GLfloat*>(data));
}

