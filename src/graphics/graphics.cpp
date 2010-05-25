#include "graphics.h"
#include <stdio.h>

#define MAX_MATERIAL_COUNT 10

namespace gfx{

  bool WorldObjectPtr_Sort_Type(WorldObject * objPtr1, WorldObject * objPtr2) {
	  return(objPtr1->getType() < objPtr2->getType());
  }
  int ObjTypeSortIndex = -1;

  Renderer renderer;
  ModelHandler modelHandler = ModelHandler();
  Hud hud;
  bool draw_wireframe;

  bool draw_axis;

  Material* materials;

  GLint shSimple;
  GLint shBuildGrid;
  GLint shForceBlock;
  GLint shCircuity;

  GLint cur_shader;

  void useShader(GLint program)
  {
    cur_shader = program;
    glUseProgram(program);
  }

  void display()
  {
    if(draw_wireframe)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    renderer.draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    hud.draw();
    glutSwapBuffers();
  }

  void reshape(int w, int h)
  {
    renderer.reshape(w, h);
  }

  void initMaterials()
  {
    materials = (Material*)malloc(sizeof(Material) * MAX_MATERIAL_COUNT);

    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];

    ambient[0] = 0.1; ambient[1] = 0.1; ambient[2] = 0.1; ambient[3] = 1.0;
    diffuse[0] = 0.4; diffuse[1] = 0.4; diffuse[2] = 0.4; diffuse[3] = 1.0;
    specular[0] = 0.1; specular[1] = 0.1; specular[2] = 0.1; specular[3] = 1.0;
    shininess[0] = 100.0;
    materials[WHITE_MAT] = Material(ambient, diffuse, specular, shininess);

    ambient[0] = 0.1; ambient[1] = 0.1; ambient[2] = 0.1; ambient[3] = 1.0;
    diffuse[0] = 0.9; diffuse[1] = 0.9; diffuse[2] = 0.1; diffuse[3] = 1.0;
    specular[0] = 0.1; specular[1] = 0.1; specular[2] = 0.1; specular[3] = 1.0;
    shininess[0] = 100.0;
    materials[YELLOW_MAT] = Material(ambient, diffuse, specular, shininess);

    ambient[0] = 0.1; ambient[1] = 0.1; ambient[2] = 0.1; ambient[3] = 1.0;
    diffuse[0] = 0.1; diffuse[1] = 0.1; diffuse[2] = 0.9; diffuse[3] = 1.0;
    specular[0] = 0.1; specular[1] = 0.1; specular[2] = 0.1; specular[3] = 1.0;
    shininess[0] = 100.0;
    materials[BLUE_MAT] = Material(ambient, diffuse, specular, shininess);

    ambient[0] = 0.1; ambient[1] = 0.1; ambient[2] = 0.1; ambient[3] = 1.0;
    diffuse[0] = 0.1; diffuse[1] = 0.9; diffuse[2] = 0.1; diffuse[3] = 1.0;
    specular[0] = 0.1; specular[1] = 0.1; specular[2] = 0.1; specular[3] = 1.0;
    shininess[0] = 100.0;
    materials[GREEN_MAT] = Material(ambient, diffuse, specular, shininess);

    ambient[0] = 0.05; ambient[1] = 0.05; ambient[2] = 0.05; ambient[3] = 1.0;
    diffuse[0] = 0.0; diffuse[1] = 0.0; diffuse[2] = 0.0; diffuse[3] = 1.0;
    specular[0] = 0.1; specular[1] = 0.1; specular[2] = 0.1; specular[3] = 1.0;
    shininess[0] = 100.0;
    materials[GRID] = Material(ambient, diffuse, specular, shininess);

    ambient[0] = 0.6; ambient[1] = 0.6; ambient[2] = 0.6; ambient[3] = 1.0;
    diffuse[0] = 0.95; diffuse[1] = 0.95; diffuse[2] = 0.95; diffuse[3] = 1.0;
    specular[0] = 0.1; specular[1] = 0.1; specular[2] = 0.1; specular[3] = 1.0;
    shininess[0] = 100.0;
    materials[GRID_DIFFUSE] = Material(ambient, diffuse, specular, shininess);
  }

  void init()
  {
    initMaterials();
    loadShaders();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    draw_wireframe = false;
    draw_axis = false;

    renderer.init();

    hud.init();
    modelHandler.initialize();
  }


  void update(int elapsed)
  {
    hud.update(elapsed);
    renderer.update(elapsed);

    if(io::keys['p'])
    {
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
    success &= installShader( "resources/shaders/buildGrid", &shBuildGrid );
    success &= installShader( "resources/shaders/forceblock", &shForceBlock );
    success &= installShader( "resources/shaders/circuity", &shCircuity );

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
