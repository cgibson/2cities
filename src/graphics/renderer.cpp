#include <GL/gl.h>
#include <GL/glut.h>
#include "renderer.h"
#include "../helper/Vector.h"
#include "../system/global.h"

#include <stdio.h>
//#include "../system/global.h"

Renderer::Renderer()
{
  eye = Vector(0,0,5);
  lookAt = Vector(0,0,0);
}

void Renderer::updateLookat()
{
  gluLookAt(eye.x(), eye.y(), eye.z(), 
            lookAt.x(), lookAt.y(), lookAt.z(), 
            0, 1, 0);
}

void Renderer::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  
  glPushMatrix();
  
  // update camera
  updateLookat();
  
  // update lights
  
  // render scene
  glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex3f(1, -1, 2);
    glColor3f(0,1,0);
    glVertex3f(-1, -1, 2);
    glColor3f(0,0,1);
    glVertex3f(0, 1, 2);
  glEnd();
  
  glPopMatrix();

  glutSwapBuffers();
}

void Renderer::reshape(int w, int h) {
  global::width = w;
  global::height = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( 45.0f,
      w/(float)h,
      0.1f,
      500.0f);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);

}
