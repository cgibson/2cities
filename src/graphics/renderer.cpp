#include "renderer.h"
#include "graphics.h"
#include "../system/enum.h"

#include "../network/NetworkPrivate.h"
#include "../scene/CustomObject.h"
#include "../state/BuildState.h"

using namespace enumeration;

Renderer::Renderer()
{
  
	// no need to init camera here anymore, it knows how to initialize itself
}

bool Renderer::initFBO()
{
  return fbo.initialize(global::width, global::height, 2);
}

void Renderer::init()
{
  init_lights();
  if(!initFBO())
  {
    printf("ERROR: fbo generation error.\n");
    exit(-1);
  }
  skybox.init();
}

void Renderer::update(int elapsed)
{
	skybox.update(elapsed);
}

void Renderer::do_lights(Shader sh)
{
  light.doLighting(sh, "light");
}

void Renderer::init_lights()
{
  std::cout << "initialized" << std::endl;
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  light.createLight(50, 50, 50,
                    0.8, 0.8, 0.8,
                    0.3,0.3,0.3);
}

void Renderer::draw()
{
  //fbo.enable();
  drawDiffusePass();
  //fbo.disable();
  
  
}

void Renderer::drawDiffusePass()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // project the camera (need to do this every frame since the 2D overlay wipes it)
    global::camera->project();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  // update lights
  do_lights(gfx::simpleShader);
  do_lights(gfx::gridShader);
  do_lights(gfx::forceBlockShader);

  glPushMatrix();
  
	if (global::stateManager->currentState->stateType() == CARNAGE_STATE)
	{
		skybox.draw(light.position[0], light.position[1], light.position[2]);
	}

  shader::reset();

    //TODO: Remove BEGIN
/*
  glDisable(GL_DEPTH_TEST);
  glColor3f(0,0,0);
  glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glVertex3f(-500, 0, -500);
    glVertex3f(-500, 0,  500);
    glVertex3f( 500, 0,  500);
    glVertex3f( 500, 0, -500);
  glEnd();
  glEnable(GL_DEPTH_TEST);
*/
  //TODO: Remove END

  
  int loc;

  gfx::gridShader.enable();

  glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
    glNormal3f(0,1,0);
    glVertex3f(-100, 0, -100);
    glVertex3f(-100, 0,  100);
    glVertex3f( 100, 0,  100);
    glVertex3f( 100, 0, -100);
  glEnd();
  
  shader::reset();

  //if(gfx::draw_axis && true)
  //{
    glEnable(GL_LINE_SMOOTH);


	  glDisable(GL_LIGHTING);
	  glDisable(GL_DEPTH_TEST);
	  glBegin(GL_LINES);
    glLineWidth(8.0);
    glPointSize(8.0);
	  glColor3f(1,0,1);
	  glVertex3f(-2,0,0);
	  glVertex3f(2,0,0);

	  glColor3f(1,1,0);
	  glVertex3f(0,-2,0);
	  glVertex3f(0,2,0);

	  glColor3f(0,1,0);
	  glVertex3f(0,0,-2);
	  glVertex3f(0,0,2);
	  glEnd();
	  glEnable(GL_DEPTH_TEST);
	  glEnable(GL_LIGHTING);
  //}

  InGameState *curstate = global::stateManager->currentState;

  float forceResult,strength;
  Vector force;
  bool changed;

  if(curstate->objects.size() > 0)
  {
	  changed = false;
	  ObjectType lastType, curType;
	  lastType = WARPED_CUBE;
	  curType = DUMMY_BLOCK;
	  Blueprint blueprint;// = global::factory->getBlueprint(curType);
	  Material curMat; // = gfx::materials[blueprint.getMaterial()];

	  vector<WorldObject *>::iterator objIt;
	  vector<WorldObject *> *objList = &curstate->objects;

	  for(objIt = objList->begin(); objIt != objList->end(); objIt++)
	  {
      curType = (*objIt)->getType();
      if(curType != lastType || !changed)
      {
        changed = true;
        lastType = curType;
        blueprint = global::factory->getBlueprint(curType);
        curMat = gfx::materials[blueprint.getMaterial()];
        curMat.applyMaterial(gfx::simpleShader, "");
        switch(curType)
        {
          case DUMMY_BLOCK:
            gfx::forceBlockShader.enable();
            break;
          case DUMMY_SPHERE: case CUSTOM_BLOCK:
            gfx::simpleShader.enable();
            blueprint = global::factory->getBlueprint(curType);
            curMat = gfx::materials[blueprint.getMaterial()];
            curMat.applyMaterial(gfx::simpleShader, "");
            break;
          default:
            //gfx::useShader(gfx::shForceBlock);
            //printf("BEFORE: %d\n", shader::current_shader);
            gfx::forceBlockShader.enable();
            //printf("AFTER: %d\n", shader::current_shader);
            /*blueprint = global::factory->getBlueprint(curType);
            curMat = gfx::materials[blueprint.getMaterial()];
            curMat.applyMaterial(gfx::simpleShader, "");*/

            break;
        }
      }
      
      if(curType != DUMMY_SPHERE)
      {
        blueprint = global::factory->getBlueprint(curType);
        curMat = gfx::materials[blueprint.getMaterial()];
        curMat.applyMaterial(gfx::forceBlockShader, "");

        loc = glGetUniformLocation(gfx::forceBlockShader.getProgram(), "force");

        force = (*objIt)->getForce();
        forceResult = force.x();
        strength = force.y() * 4.0;
        glUniform1f(loc, forceResult);
        loc = glGetUniformLocation(gfx::forceBlockShader.getProgram(), "shock");
        glUniform1f(loc, strength);
      }

      (*objIt)->draw();
	  }
	}

  shader::reset();
  
  glUseProgram(0);
  
  if(curstate->stateType() == BUILD_STATE)
  {

    //BuildState *bs = (BuildState*)curstate;
    if(BuildStateGlobals::renderPlane)
    {
      gfx::modelHandler.drawPlane(
                          BuildStateGlobals::planeNormal, 
                          BuildStateGlobals::planeLocation, 
                          BuildStateGlobals::planeSize);
    }
  }

  glPopMatrix();

}

void Renderer::getMatrices(GLdouble **mvMatrix, GLdouble **pMatrix, GLint **vMatrix)
{
    global::camera->project();

	//GLdouble modelview_matrix[16];
	//GLdouble projection_matrix[16];

	glGetDoublev( GL_MODELVIEW_MATRIX, *mvMatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, *pMatrix );
	glGetIntegerv( GL_VIEWPORT, *vMatrix );

}

void Renderer::reshape(int w, int h) {
  global::width = w;
  global::height = h;

  glViewport(0, 0, w, h);
}
