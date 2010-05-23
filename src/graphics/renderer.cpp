#include "renderer.h"
#include "graphics.h"
#include "../system/enum.h"

#include "../network/NetworkPrivate.h"

using namespace enumeration;

Renderer::Renderer()
{
	// no need to init camera here anymore, it knows how to initialize itself
}

void Renderer::init()
{
  init_lights();
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

  // update lights
  do_lights();

  glPushMatrix();

  //gfx::useShader(gfx::shSimple);
  gfx::useShader(gfx::shSimple);
  //TODO: Remove BEGIN

  gfx::materials[WHITE_MAT].applyMaterial(gfx::cur_shader, "material");

  glBegin(GL_QUADS);
    glNormal3f(0,1,0);
    glVertex3f(-100, 0, -100);
    glVertex3f(-100, 0,  100);
    glVertex3f( 100, 0,  100);
    glVertex3f( 100, 0, -100);
  glEnd();

  //TODO: Remove END


  int loc;
  InGameState *curstate = global::stateManager->currentState;

  if(curstate->objects.size() > 0)
  {
	  ObjectType lastType, curType;
	  lastType = WARPED_CUBE;
	  curType = curstate->objects[0]->getType();
	  Blueprint blueprint = global::factory->getBlueprint(curType);
	  Material curMat = gfx::materials[blueprint.getMaterial()];

	  curMat.applyMaterial(gfx::cur_shader, "");
	  //printf("SOMETHING HERE\n");
	  for(int i = 0; i < (int)curstate->objects.size(); i++)
	  {
		curType = curstate->objects[i]->getType();
		if(curType != lastType)
		{
			switch(curType)
			{
				case DUMMY_BLOCK:
					gfx::useShader(gfx::shForceBlock);

					break;
				case DUMMY_SPHERE:
				default:
					gfx::useShader(gfx::shSimple);
					break;
			}
			//printf("DIFFERENCE!\n");
			lastType = curType;
			blueprint = global::factory->getBlueprint(curType);
			curMat = gfx::materials[blueprint.getMaterial()];
			curMat.applyMaterial(gfx::cur_shader, "");
		}
		loc = glGetUniformLocation(gfx::shForceBlock, "force");
		Vector force = curstate->objects[i]->getForce();
		float forceResult = force.x() * 40.0f;
		float strength = force.y() * 2.0;
		//printf("strength: %f\n", strength);
		glUniform1f(loc, forceResult);
		loc = glGetUniformLocation(gfx::shForceBlock, "edge_strength");
		glUniform1f(loc, 0.2 + strength);


		curstate->objects[i]->draw();
	  }
	}

  glUseProgram( 0 );

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
