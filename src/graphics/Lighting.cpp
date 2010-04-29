#include "Lighting.h"
#include "../helper/Vector.h"
#include <GL/glut.h>
#include <stdio.h>
LightData lights[MAX_LIGHTS];

LightData::LightData()
{
 	
}

LightData::~LightData()
{
 	
}

int num_lights = 0;

void Lighting::createLight(float x, float y, float z,
	    float Dr, float Dg, float Db, float spec,
	    float Ar, float Ag, float Ab,
	    float cA, float lA, float qA)
{
  lights[num_lights].pos[0] = x;
  lights[num_lights].pos[1] = y;
  lights[num_lights].pos[2] = z;
  lights[num_lights].pos[3] = 1.0f;

  lights[num_lights].diffuse[0] = Dr;
  lights[num_lights].diffuse[1] = Dg;
  lights[num_lights].diffuse[2] = Db;
  lights[num_lights].diffuse[3] = 1.0f;

  lights[num_lights].ambient[0] = Ar;
  lights[num_lights].ambient[1] = Ag;
  lights[num_lights].ambient[2] = Ab;
  lights[num_lights].ambient[3] = 1.0f;

  lights[num_lights].constantAttenuation = cA;
  lights[num_lights].linearAttenuation = lA;
  lights[num_lights].quadraticAttenuation = qA;

  num_lights++;
}

void Lighting::setLightPos(int index, float x, float y, float z)
{
  lights[index].pos[0] = x;
  lights[index].pos[1] = y;
  lights[index].pos[2] = z;
}

void Lighting::doLighting() {

  for(int i=0; i<num_lights; i++)
  {
    glLightfv(GL_LIGHT0+i, GL_POSITION, lights[i].pos);
    glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lights[i].diffuse);
    glLightfv(GL_LIGHT0+i, GL_AMBIENT, lights[i].ambient);
    glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, lights[i].constantAttenuation);
    glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, lights[i].linearAttenuation);
    glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, lights[i].quadraticAttenuation);
  }
}

