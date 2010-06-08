#include "renderer.h"
#include "graphics.h"
#include "../system/enum.h"

#include "../network/NetworkPrivate.h"
#include "../scene/CustomObject.h"
#include "../state/BuildState.h"

using namespace enumeration;

const int Renderer::min_test = 1;
const int Renderer::max_test = 8;

Renderer::Renderer()
{
	// no need to init camera here anymore, it knows how to initialize itself
	hackyRotate = 0.0;
}

void Renderer::init()
{
  init_lights();
  skybox.init();

  // use for adaptive bloom
  test_count = 3;
	fpsQueue = new LimitedQueue(50);


	gfx::simpleScreenFillShader.enable();

	int loc = glGetUniformLocation(gfx::simpleScreenFillShader.getProgram(), "test_count");
	glUniform1i(loc, test_count);

	shader::reset();

}

void Renderer::updateBloom(int elapsed)
{

	fpsQueue->add(elapsed);

	float average = 0;
	for(int i = 0; i < fpsQueue->size(); i++)
	{
		average += (float)fpsQueue->get(i);
	}

	average /= fpsQueue->size();

	average = 1000.0 / average;

	if((average < 25.0) && (test_count > min_test) && (fpsQueue->size() > 1))
	{
		test_count--;
    
#ifdef DEBUG
		printf("frame average: %.1f, so lowered tests to %d\n", average, test_count);
#endif

		gfx::simpleScreenFillShader.enable();

		int loc = glGetUniformLocation(gfx::simpleScreenFillShader.getProgram(), "test_count");
    glUniform1i(loc, test_count);

		shader::reset();

	}else if((average > 60.0) && (test_count < max_test) && (fpsQueue->size() > 1))
	{
		test_count++;

#ifdef DEBUG
		printf("frame average: %.1f, so raised tests to %d\n", average, test_count);
#endif

		gfx::simpleScreenFillShader.enable();

		int loc = glGetUniformLocation(gfx::simpleScreenFillShader.getProgram(), "test_count");
    glUniform1i(loc, test_count);
    
		shader::reset();
	}
}

void Renderer::update(int elapsed)
{
	skybox.update(elapsed);

	if(global::stateManager->currentState->stateType() == CARNAGE_STATE)
		updateBloom(elapsed);

	hackyRotate += 0.15;
	while (hackyRotate > 360.0) { hackyRotate -= 360.0; }
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

void Renderer::draw_screen()
{
  gfx::simpleScreenFillShader.update();
  gfx::simpleScreenFillShader.enable();

  glBegin(GL_QUADS);
	glColor4f(1,1,1,1);
    glTexCoord2f(0,0);
    glVertex3f(-1.0, -1.0, 0);
    glTexCoord2f(1,0);
    glVertex3f(1.0, -1.0, 0);
    glTexCoord2f(1 ,1);
    glVertex3f(1.0, 1.0, 0);
    glTexCoord2f(0 ,1);
    glVertex3f(-1.0, 1.0, 0);
  glEnd();
}

void Renderer::draw()
{
  if((gfx::renderState == FULL) &&
      (global::stateManager->currentState->stateType() == CARNAGE_STATE))
  {
	// render the scene into the FBO (first pass)
	glEnable(GL_DEPTH_TEST);
    gfx::fbo->enable();
    draw_diffusePass();
    gfx::fbo->disable();

    shader::reset();

    // clear the current
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw second pass
    draw_screen();

    shader::reset();
    //draw_diffusePass();


  }else{
    draw_diffusePass();
  }


}

void Renderer::draw_diffusePass()
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
  do_lights(gfx::ammoShader);

  glPushMatrix();

	if (global::stateManager->currentState->stateType() == CARNAGE_STATE ||
		global::stateManager->currentState->stateType() == MENU_STATE ||
		global::stateManager->currentState->stateType() == RESULTS_STATE)
	{
		skybox.draw(light.position[0], light.position[1], light.position[2], false);
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
          case DUMMY_BLOCK: case BLOCK_1_2_1: case BLOCK_2_4_2: case BLOCK_5_1_5:
            gfx::forceBlockShader.enable();
            if((*objIt)->getPhysics() == STATIC)
            {
							curMat = gfx::materials[DEAD_BLOCK];
						}else if((*objIt)->getPlayerID() == 1)
            {
              curMat = gfx::materials[RED_BLOCK];
						}else{
							curMat = gfx::materials[BLUE_BLOCK];
						}
            curMat.applyMaterial(gfx::forceBlockShader, "");
            
            loc = glGetUniformLocation(gfx::forceBlockShader.getProgram(), "force");

            force = (*objIt)->getForce();
            forceResult = force.x();
            strength = force.y() * 4.0;
            glUniform1f(loc, forceResult);
            loc = glGetUniformLocation(gfx::forceBlockShader.getProgram(), "shock");
            glUniform1f(loc, strength);
            
            break;
          case CUSTOM_BLOCK:
            gfx::simpleShader.enable();
            blueprint = global::factory->getBlueprint(curType);
            curMat = gfx::materials[blueprint.getMaterial()];
            curMat.applyMaterial(gfx::simpleShader, "");
            break;
          case DUMMY_SPHERE: 
            gfx::ammoShader.enable();
            gfx::materials[YELLOW_MAT].applyMaterial(gfx::ammoShader, "");
            break;
          case BALLHEMOTH: 
            gfx::ammoShader.enable();
            gfx::materials[YELLOW_MAT].applyMaterial(gfx::ammoShader, "");
            break;
          case BLACK_HOLE: 
            gfx::ammoShader.enable();
            gfx::materials[YELLOW_MAT].applyMaterial(gfx::ammoShader, "");
            break;
          case AIR_STRIKE:
            gfx::ammoShader.enable();
            gfx::materials[YELLOW_MAT].applyMaterial(gfx::ammoShader, "");
            break;
          case SHOTGUN:
            gfx::ammoShader.enable();
            gfx::materials[YELLOW_MAT].applyMaterial(gfx::ammoShader, "");
            break;
          case SHAPE_SHIFTER:
            gfx::ammoShader.enable();
            gfx::materials[YELLOW_MAT].applyMaterial(gfx::ammoShader, "");
            break;
          case CLUSTER_BOMB:
            gfx::ammoShader.enable();
            gfx::materials[YELLOW_MAT].applyMaterial(gfx::ammoShader, "");
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

      /*if(curType == DUMMY_SPHERE)
      {*/
        /*blueprint = global::factory->getBlueprint(curType);
        curMat = gfx::materials[blueprint.getMaterial()];
        curMat.applyMaterial(gfx::forceBlockShader, "");*/
/*
        loc = glGetUniformLocation(gfx::forceBlockShader.getProgram(), "force");

        force = (*objIt)->getForce();
        forceResult = force.x();
        strength = force.y() * 4.0;
        glUniform1f(loc, forceResult);
        loc = glGetUniformLocation(gfx::forceBlockShader.getProgram(), "shock");
        glUniform1f(loc, strength);
      }*/

      (*objIt)->draw();
	  }
	}
  shader::reset();

  glUseProgram(0);

	CarnageState *cs;

  switch(curstate->stateType())
  {
    case BUILD_STATE:
			//BuildState *bs = (BuildState*)curstate;
			if(BuildStateGlobals::renderPlane)
			{
				gfx::modelHandler.drawPlane(
														BuildStateGlobals::planeNormal,
														BuildStateGlobals::planeLocation,
														BuildStateGlobals::planeSize);
			}
    break;
    case CARNAGE_STATE:
      cs = (CarnageState*)curstate;
      gfx::simpleShader.enable();
			cs->opponentMat->applyMaterial(gfx::simpleShader, "");

			cs->opponent->draw();

			// render current cannon
			glPushMatrix();
				glClear(GL_DEPTH_BUFFER_BIT);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(45.0,
											(float)global::width / global::height,
											1.0, 500.0);
				gluLookAt(0, 0, 2,
									0, 0, 5,
									0, 1, 0);
				glMatrixMode(GL_MODELVIEW);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				glTranslatef(0,-1.3,3.0);
				glRotatef(hackyRotate, 0,0,1);

				cs->playerMat->applyMaterial(gfx::simpleShader, "");
				cs->playerCannon->draw();
			glPopMatrix();

    break;
    default: break;
  }

  shader::reset();

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
