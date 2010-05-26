#include "modelhandler.h"

ModelHandler::ModelHandler()
{
}

void ModelHandler::drawShadedCube(float size)
{
	glPushMatrix();
		glScalef(size/2.0,size/2.0,size/2.0);
		glCallList(shadedCubeList);
	glPopMatrix();
}

void ModelHandler::drawPlane(Vector n, Vector p, float size)
{
  Vector offset;
  bool horizontal = false;
  if(n == Vector(0,1,0))
  {
    horizontal = true;
    offset = Vector(1,0,1);
  }
  else
  {
    offset = n.cross(Vector(0,1,0));
    offset = offset + Vector(0,1,0);
  }
    
  offset = offset * size;
  
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  if(horizontal)
  {
    glBegin(GL_QUADS);
      glColor4f(0,0,1,0.1);
      glVertex3f(p.x() + offset.x(), p.y(), p.z() + offset.z());
      glVertex3f(p.x() + offset.x(), p.y(), p.z() - offset.z());
      glVertex3f(p.x() - offset.x(), p.y(), p.z() - offset.z());
      glVertex3f(p.x() - offset.x(), p.y(), p.z() + offset.z());
    glEnd();

  }else{
    glBegin(GL_QUADS);
      glColor4f(0,0,1,0.1);
      glVertex3f(p.x() + offset.x(), p.y() + offset.y(), p.z() + offset.z());
      glVertex3f(p.x() - offset.x(), p.y() + offset.y(), p.z() - offset.z());
      glVertex3f(p.x() - offset.x(), p.y() - offset.y(), p.z() - offset.z());
      glVertex3f(p.x() + offset.x(), p.y() - offset.y(), p.z() + offset.z());
    glEnd();
  }
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
}

void ModelHandler::initialize()
{
	shadedCubeList = genShadedCubeDL();
	printf("list1: %d\n", shadedCubeList);
}

GLuint ModelHandler::genShadedCubeDL()
{
	GLuint list = glGenLists(1);
	
	if(!list)
	{
		printf("Error: unable to generate cube display list\n");
		exit(-1);
	}
	
	glNewList(list, GL_COMPILE);
	
	glBegin(GL_QUADS);
	//FRONT FACE
		glNormal3f( 0,  0,  1);
		glTexCoord2f(0,0);
		glVertex3f(-1, -1,  1);
		glTexCoord2f(1,0);
		glVertex3f( 1, -1,  1);
		glTexCoord2f(1,1);
		glVertex3f( 1,  1,  1);
		glTexCoord2f(0,1);
		glVertex3f(-1,  1,  1);
	
	//BACK FACE
		glNormal3f( 0,  0, -1);
		glTexCoord2f(0,0);
		glVertex3f(-1, -1, -1);
		glTexCoord2f(1,0);
		glVertex3f( 1, -1, -1);
		glTexCoord2f(1,1);
		glVertex3f( 1,  1, -1);
		glTexCoord2f(0,1);
		glVertex3f(-1,  1, -1);
	
	//LEFT FACE
		glNormal3f(-1,  0,  0);
		glTexCoord2f(0,0);
		glVertex3f(-1, -1, -1);
		glTexCoord2f(1,0);
		glVertex3f(-1, -1,  1);
		glTexCoord2f(1,1);
		glVertex3f(-1,  1,  1);
		glTexCoord2f(0,1);
		glVertex3f(-1,  1, -1);
	
	//RIGHT FACE
		glNormal3f( 1,  0,  0);
		glTexCoord2f(0,0);
		glVertex3f( 1, -1, -1);
		glTexCoord2f(1,0);
		glVertex3f( 1, -1,  1);
		glTexCoord2f(1,1);
		glVertex3f( 1,  1,  1);
		glTexCoord2f(0,1);
		glVertex3f( 1,  1, -1);
	
	//TOP FACE
		glNormal3f(0,1,0);
		glTexCoord2f(0,0);
		glVertex3f(-1,  1, -1);
		glTexCoord2f(1,0);
		glVertex3f( 1,  1, -1);
		glTexCoord2f(1,1);
		glVertex3f( 1,  1,  1);
		glTexCoord2f(0,1);
		glVertex3f(-1,  1,  1);
	
	//BOTTOM FACE
		glNormal3f( 0, -1,  0);
		glTexCoord2f(0,0);
		glVertex3f(-1, -1, -1);
		glTexCoord2f(1,0);
		glVertex3f( 1, -1, -1);
		glTexCoord2f(1,1);
		glVertex3f( 1, -1,  1);
		glTexCoord2f(0,1);
		glVertex3f(-1, -1,  1);
		
	glEnd();
	
	glEndList();
	
	return list;
}
