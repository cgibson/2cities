#include <GL/gl.h>
#include <GL/glut.h>
#include "renderer.h"
#include "../helper/Vector.h"
#include "../system/global.h"
#include "../state/CarnageState.h"

#include <stdio.h>
#include <iostream>
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
  
  Vector pos;
  Vector axis;
  float angle;
  
  for(int i = 0; i < scene.size(); i++)
  {
    BuildingUnit unit = scene[i];
    pos = unit.getPosition();
    axis = unit.getRotation();
    angle = unit.getRotationMag();
    std::cout << "Axis:" << axis.x() << " " << axis.y() << " " << axis.z() << std::endl;
    std::cout << "Angle: " << angle << std::endl;
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());
    //glRotatef(angle, axis.x(), axis.y(), axis.z());
    glColor3f(1,0,0);
    glutSolidCube(1);
    glPopMatrix();
  }
  
  glPopMatrix();
}

void Renderer::reshape(int w, int h) {
  global::width = w;
  global::height = h;
  
  glViewport(0, 0, w, h);
}
