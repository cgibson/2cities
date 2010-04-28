#include <GL/gl.h>
#include <GL/glut.h>
#include "renderer.h"
#include "../helper/Vector.h"
#include "../system/global.h"

#include <stdio.h>
//#include "../system/global.h"

Renderer::Renderer()
{
  global::camera.eye = Vector(0,0,5);
  global::camera.lookAt = Vector(0,0,0);
  global::camera.fov = 45.0f;
  global::camera.near = 0.1f;
  global::camera.far = 100.0f;
  
}

void Renderer::updateLookat()
{
  gluLookAt(global::camera.eye.x(), global::camera.eye.y(), global::camera.eye.z(), 
            global::camera.lookAt.x(), global::camera.lookAt.y(), global::camera.lookAt.z(), 
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
}

void Renderer::reshape(int w, int h) {
  global::width = w;
  global::height = h;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective( global::camera.fov,
      w/(float)h,
      global::camera.near,
      global::camera.far);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, w, h);

}
