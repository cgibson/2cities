#include <GL/gl.h>
#include <GL/glut.h>

#include "graphics.h"
#include "../system/io.h"


namespace gfx{

  Renderer renderer;
  Hud hud;
  bool draw_wireframe;

  void display()
  {
    renderer.draw();
    hud.draw();
    glutSwapBuffers();
  }
  
  void reshape(int w, int h)
  {
    renderer.reshape(w, h);
  }
  

  void init()
  {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    draw_wireframe = false;

    hud.init();
  }
  
  
  void update(int elapsed)
  {
    hud.update(elapsed);
    
    if(io::keys['p'])
    {
      if(draw_wireframe)
      {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
        draw_wireframe = !draw_wireframe;
    }

    display();
  }
  
  
}
