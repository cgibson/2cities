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
  EffectManager fxManager;
  bool draw_wireframe;

  bool draw_axis;

  Material* materials;
  
  RenderState renderState = LIMITED;

  DefaultShader simpleShader;
  DefaultShader gridShader;
  DefaultShader forceBlockShader;
  DefaultShader skyscraperShader;
  DefaultShader skyShader;
  DefaultShader distantShader;
  DefaultShader uiIconShader;
  
  PassShader simpleScreenFillShader;
  
  FBOHelper *fbo;
  
  
  bool initFBO()
  {
    gfx::fbo = new FBOHelper();
    return gfx::fbo->initialize(global::width, global::height, 2);
  }

  void display()
  {
    if(draw_wireframe)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    renderer.draw();
    fxManager.draw();

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
	if(renderState == FULL)
	{
	  initFBO();
    }
    initMaterials();
    loadShaders();
    loadDefaultShaderValues();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    draw_wireframe = false;
    draw_axis = false;

    renderer.init();

    hud.init();
    modelHandler.initialize();
    fxManager.init();
  }


  void update(int elapsed)
  {
    hud.update(elapsed);
    renderer.update(elapsed);
    fxManager.update(elapsed);

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

    simpleShader.load("resources/shaders/simpleShade");
    gridShader.load("resources/shaders/buildGrid");
    forceBlockShader.load("resources/shaders/forceblock");
    skyscraperShader.load("resources/shaders/skyscraper");
    skyShader.load("resources/shaders/sky");
    distantShader.load("resources/shaders/distant");
    uiIconShader.load("resources/shaders/uiicon");
    simpleScreenFillShader.load("resources/shaders/simpleFill");

    if(!success)
    {
      exit(-1);
    }

    return success;
  }

  void loadDefaultShaderValues()
  {
    GLint old = shader::current_shader;
    gridShader.enable();

    materials[GRID].applyMaterial(gridShader, "material");

    int loc = glGetUniformLocation(gridShader.getProgram(), "grid_diffuse");
    glUniform4fv(loc, 1, materials[GRID_DIFFUSE].diffuse);

    loc = glGetUniformLocation(gridShader.getProgram(), "grid_size");
    glUniform1f(loc, 1.0f);

    loc = glGetUniformLocation(gridShader.getProgram(), "line_pct");
    glUniform1f(loc, 0.03f);

    loc = glGetUniformLocation(gridShader.getProgram(), "field_width");
    glUniform1f(loc, global::map_width);

    loc = glGetUniformLocation(gridShader.getProgram(), "field_depth");
    glUniform1f(loc, global::map_height);

    loc = glGetUniformLocation(gridShader.getProgram(), "border_size");
    glUniform1f(loc, 1);

    loc = glGetUniformLocation(gridShader.getProgram(), "division_size");
    glUniform1f(loc, 5);
    
    if(renderState == FULL)
    {
      simpleScreenFillShader.setTextures(fbo->getTextureIDs(), 1);
      simpleScreenFillShader.enable();
    }

    shader::setProgram(old);

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
