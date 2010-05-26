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

void Renderer::init()
{
  init_lights();
  skybox.init();
}

void Renderer::update(int elapsed)
{
	skybox.update(elapsed);
}

void Renderer::do_lights()
{
  light.doLighting(gfx::cur_shader, "light");
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


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // project the camera (need to do this every frame since the 2D overlay wipes it)
    global::camera->project();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  //gfx::useShader( 0 );

  // update lights
  do_lights();

  glPushMatrix();

    //TODO: Remove BEGIN

  gfx::useShader( 0 );

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

  gfx::useShader( 0 );

  if(gfx::draw_axis)
  {
	  glDisable(GL_LIGHTING);
	  glDisable(GL_DEPTH_TEST);
	  glBegin(GL_LINES);
	  glColor3f(1,0,1);
	  glVertex3f(0,0,0);
	  glVertex3f(4,0,0);

	  glColor3f(1,1,0);
	  glVertex3f(0,0,0);
	  glVertex3f(0,4,0);

	  glColor3f(0,1,0);
	  glVertex3f(0,0,0);
	  glVertex3f(0,0,4);
	  glEnd();
	  glEnable(GL_DEPTH_TEST);
	  glEnable(GL_LIGHTING);
  }

  //TODO: Remove END

  int loc;

	if (global::stateManager->currentState->stateType() == CARNAGE_STATE)
	{
		gfx::useShader(gfx::shCircuity);
		skybox.draw(light.position[0], light.position[1], light.position[2]);
	}

  //gfx::useShader(gfx::shSimple);
  //switch(global::stateManager->currentState->stateType())
  //{
	//case BUILD_STATE:

		gfx::useShader(gfx::shBuildGrid);

		gfx::materials[GRID].applyMaterial(gfx::shBuildGrid, "material");

	    loc = glGetUniformLocation(gfx::shBuildGrid, "grid_diffuse");
		glUniform4fv(loc, 1, gfx::materials[GRID_DIFFUSE].diffuse);

	    loc = glGetUniformLocation(gfx::shBuildGrid, "grid_size");
		glUniform1f(loc, 1.0f);

		loc = glGetUniformLocation(gfx::shBuildGrid, "line_pct");
		glUniform1f(loc, 0.03f);
	//	break;
	//case CARNAGE_STATE: default:
		//gfx::materials[WHITE_MAT].applyMaterial(gfx::cur_shader, "material");
		//gfx::useShader(gfx::shSimple);

	/*	gfx::useShader(gfx::shBuildGrid);

		gfx::materials[GRID].applyMaterial(gfx::shBuildGrid, "material");

	    loc = glGetUniformLocation(gfx::shBuildGrid, "grid_diffuse");
		glUniform4fv(loc, 1, gfx::materials[GRID_DIFFUSE].diffuse);

	    loc = glGetUniformLocation(gfx::shBuildGrid, "grid_size");
		glUniform1f(loc, 1.0f);

		loc = glGetUniformLocation(gfx::shBuildGrid, "line_pct");
		glUniform1f(loc, 0.03f);
		break;*/
  //}

  //printf("WTF IS THIS? %d\n", gfx::cur_shader);

  glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
    glNormal3f(0,1,0);
    glVertex3f(-100, 0, -100);
    glVertex3f(-100, 0,  100);
    glVertex3f( 100, 0,  100);
    glVertex3f( 100, 0, -100);
  glEnd();

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
	  Blueprint blueprint = global::factory->getBlueprint(curType);
	  Material curMat = gfx::materials[blueprint.getMaterial()];

	  curMat.applyMaterial(gfx::cur_shader, "");
	  //printf("SOMETHING HERE\n");
	  vector<WorldObject *>::iterator objIt;
	  vector<WorldObject *> *objList = &curstate->objects;

	  for(objIt = objList->begin(); objIt != objList->end(); objIt++)
	  {
		curType = (*objIt)->getType();
		if(curType != lastType || !changed)
		{
			changed = true;
			lastType = curType;
			switch(curType)
			{
				case DUMMY_BLOCK:
					gfx::useShader(gfx::shForceBlock);
					break;
				case DUMMY_SPHERE:
					gfx::useShader(gfx::shSimple);
					blueprint = global::factory->getBlueprint(curType);
					curMat = gfx::materials[blueprint.getMaterial()];
					curMat.applyMaterial(gfx::cur_shader, "");
					break;
				default:
					//gfx::useShader(gfx::shForceBlock);
					//printf("BEFORE: %d\n", gfx::cur_shader);
					gfx::useShader(gfx::shSimple);
					//printf("AFTER: %d\n", gfx::cur_shader);
					blueprint = global::factory->getBlueprint(curType);
					curMat = gfx::materials[blueprint.getMaterial()];
					curMat.applyMaterial(gfx::cur_shader, "");

					break;
			}
			//printf("DIFFERENCE!\n");
		}

        if(curType == DUMMY_BLOCK)
        {


			blueprint = global::factory->getBlueprint(curType);
			curMat = gfx::materials[blueprint.getMaterial()];
			curMat.applyMaterial(gfx::cur_shader, "");

			loc = glGetUniformLocation(gfx::shForceBlock, "force");

			force = (*objIt)->getForce();
			forceResult = force.x();
			strength = force.y() * 4.0;
			glUniform1f(loc, forceResult);
			loc = glGetUniformLocation(gfx::shForceBlock, "shock");
			glUniform1f(loc, strength);
		}

		(*objIt)->draw();
                    glUseProgram(0);
	  }
	}else{
			//curstate->objects.push_back(new  WorldObject(0,0,DUMMY_SPHERE));
			//curstate->objects.push_back(new CustomObject(0, 0, CUSTOM_BLOCK, Point(5, 10, 5), Point(-5, 0, -5)));
		}

  //gfx::useShader( 0 );

  glUseProgram(0);
  
  
  if(curstate->stateType() == BUILD_STATE)
  {
    //gfx::useShader( 0 );

    BuildState *bs = (BuildState*)curstate;
    if(BuildStateGlobals::renderPlane)
    {
      gfx::modelHandler.drawPlane(
                          BuildStateGlobals::planeNormal, 
                          BuildStateGlobals::planeLocation, 
                          BuildStateGlobals::planeSize);
    }
  }
  
  glUseProgram(0);

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
