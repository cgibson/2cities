#include <GL/gl.h>
#include <GL/glut.h>
#include "renderer.h"
#include "../helper/Vector.h"
#include "../system/global.h"
#include "../state/CarnageState.h"

#include <stdio.h>
#include <vector>
//#include "../system/global.h"

Renderer::Renderer()
{
  global::camera.eye = Vector(0,0,30);
  global::camera.lookAt = Vector(0,0,0);
  global::camera.fov = 45.0f;
  global::camera.near = 1.0f;
  global::camera.far = 500.0f;
  
  DEBUG_THETA = 0.0f;
  
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

    // project the camera (need to do this every frame since the 2D overlay wipes it)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(global::camera.fov,
        global::width / (float)global::height,
        global::camera.near,
        global::camera.far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  glPushMatrix();
  
  // update camera
  updateLookat();
  
  // update lights
  
  // render scene
  /*glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex3f(1, -1, 2);
    glColor3f(0,1,0);
    glVertex3f(-1, -1, 2);
    glColor3f(0,0,1);
    glVertex3f(0, 1, 2);
  glEnd();
  */
  
  CarnageState *state = (CarnageState*)global::stateManager.currentState;
  
  vector<BuildingUnit> scene = state->physics.getBuildingBlocks();
  
  glDisable(GL_LIGHTING);
  
  DEBUG_THETA+= 0.1f;
  
  Vector pos;
  
  for(int i = 0; i < scene.size(); i++)
  {
    BuildingUnit unit = scene[i];
    pos = unit.getPosition();
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());
    glRotatef(DEBUG_THETA, 0, 1, 0);
    glColor3f(1,0,0);
    glutSolidCube(20);
    glPopMatrix();
  }
  
  glPopMatrix();
}

void Renderer::reshape(int w, int h) {
  global::width = w;
  global::height = h;
  
  glViewport(0, 0, w, h);
}
