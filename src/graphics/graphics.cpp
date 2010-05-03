#include "graphics.h"

namespace gfx{

  Renderer renderer;
  Hud hud;
  bool draw_wireframe;
  
  GLint shSimple;

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
    loadShaders();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    draw_wireframe = false;
    
    renderer.init();

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
  
  /* Load all shaders for the program */
  bool loadShaders()
  {
    bool success = true;
    getGLversion();
    success &= installShader( "resources/shaders/simpleShade", &shSimple );
    
    if(!success)
    {
      exit(-1);
    }
    
    return success;
  }
  

  bool installShader( string filename, int *shaderID )
  {
    string vert = filename;
    string frag = filename;
    vert.append(".vert");
    frag.append(".frag");
    
    printf("Loading %s\n", vert.c_str());
    printf("Loading %s\n", frag.c_str());
    
	  //install the shader
	  if (InstallShader(textFileRead(vert), textFileRead(frag), shaderID) == -1) {
		  printf("Error installing shader!\n");
		  return 0;
	  }
	  return true;
  }
  
}
