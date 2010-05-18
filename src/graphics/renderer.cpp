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
    glVertex3f(-100, -1, -100);
    glVertex3f(-100, -1, 100);
    glVertex3f(100, -1, 100);
    glVertex3f(100, -1, -100);
  glEnd();

  //TODO: Remove END
  
  InGameState *curstate = global::stateManager->currentState;
  
  //ObjectType lastType, curType;
  //curType = lastType = curstate->objects[0].getType();
  
  for(int i = 0; i < (int)curstate->objects.size(); i++)
  {
    /*curType = curState->objects[i].getType();
    if(curType != lastType)
    {
    
    }*/
    curstate->objects[i]->draw();
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
