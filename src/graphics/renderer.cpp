#include "renderer.h"
#include "graphics.h"

#include "../network/NetworkPrivate.h"

Renderer::Renderer()
{
  global::camera.eye = Vector(0,0,30);
  global::camera.lookAt = Vector(0,0,0);
  global::camera.fov = 45.0f;
  global::camera.near_plane = 1.0f;
  global::camera.far_plane = 500.0f;
}

void Renderer::init()
{
  init_lights();
}

void Renderer::updateLookat()
{
  gluLookAt(global::camera.eye.x(), global::camera.eye.y(), global::camera.eye.z(), 
            global::camera.lookAt.x(), global::camera.lookAt.y(), global::camera.lookAt.z(), 
            0, 1, 0);
}

void Renderer::do_lights()
{
  light.doLighting();
}

void Renderer::init_lights()
{
  std::cout << "initialized" << std::endl;
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  light.createLight(50, 50, 50,
                    0.8, 0.8, 0.8, 1,
                    0.3,0.3,0.3,
                    1.0, 1.0, 0.0);
}

void Renderer::draw()
{
  
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // project the camera (need to do this every frame since the 2D overlay wipes it)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(global::camera.fov,
        global::width / (float)global::height,
        global::camera.near_plane,
        global::camera.far_plane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
  // update lights
  do_lights();

  glPushMatrix();
  
  // update camera
  updateLookat();
  
  glUseProgram(gfx::shSimple);
  
  //TODO: Remove BEGIN
  
  gfx::materials[WHITE_MAT].applyMaterial();
  glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glVertex3f(-100, -1, -100);
    glVertex3f(-100, -1, 100);
    glVertex3f(100, -1, 100);
    glVertex3f(100, -1, -100);
  glEnd();

  //TODO: Remove END
  
  InGameState *curstate = global::stateManager->currentState;
  
  for(int i = 0; i < (int)curstate->objects.size(); i++)
  {
    curstate->objects[i]->draw();
  }
  
  glUseProgram( 0 );  
  
  glPopMatrix();
}

void Renderer::reshape(int w, int h) {
  global::width = w;
  global::height = h;
  
  glViewport(0, 0, w, h);
}
