#include "renderer.h"
#include "graphics.h"
#include "../system/enum.h"

#include "../network/NetworkPrivate.h"

using namespace enumeration;

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
  gfx::hud.console.registerCmd("matrices", Renderer::outputMatrices);
}

void Renderer::updateLookat()
{
  gluLookAt(global::camera.eye.x(), global::camera.eye.y(), global::camera.eye.z(), 
            global::camera.lookAt.x(), global::camera.lookAt.y(), global::camera.lookAt.z(), 
            0, 1, 0);
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

void Renderer::outputMatrices(int argc, char* argv[])
{
	GLdouble modelview_matrix[16];
	GLdouble projection_matrix[16];

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview_matrix );
	glGetDoublev( GL_PROJECTION_MATRIX, projection_matrix );
int i,j;
printf("modelview:\n");	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%f ", modelview_matrix[i * 4 + j]);
		}		
		printf("\n");
	}
	printf("\nprojection:\n");	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%f ", projection_matrix[i * 4 + j]);
		}
		printf("\n");
	}
}

void Renderer::getMatrices(GLdouble **mvMatrix, GLdouble **pMatrix, GLint **vMatrix)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(global::camera.fov,
        global::width / (float)global::height,
        global::camera.near_plane,
        global::camera.far_plane);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  // update camera
  updateLookat();

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
