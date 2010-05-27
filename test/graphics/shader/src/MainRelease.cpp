// STANDARD LIBRARIES
#include <stdio.h>
#include <string>
#include <vector> 
#include <math.h>

// OPENGL INCLUDES
#include <GL/gl.h>
#include <GL/glut.h>

// UTILITY INCLUDES
#include "include/tex.h"          // texture library
#include "include/GLSL_helper.h"  // glsl helper functions
#include "include/Vector.h"
#include "include/shader.h"

Vector gLightPos;
Vector gCameraPos;
Vector gLookAt;

float theta = 0;

#define TIMESTEP 30.0f

#define GROUND_WIDTH 15.0f

#define GROUND_TEX_SIZE 5.0f

#define RENDER_WIDTH 512
#define RENDER_HEIGHT 512

/* GLOBAL VARIABLES ------------------*/
int gWidth;             // window width
int gHeight;            // window height

//GLuint diffuseTexture;
//GLuint randomTexture;

// shaders
DefaultShader *diffuseShader;
PassShader *bloomShader;
FBOHelper bloomFBOhelper;

using namespace std;

void updateView( Vector eye, Vector lookAt);

/* RENDERING ELEMENTS - All functions dealing with rendering the scene
 *----------------------------------------------------------------------------*/

/* Draw ground and walls */
void drawGround( void )
{
  float groundTexSize = GROUND_WIDTH / GROUND_TEX_SIZE;

	glColor4f(0.1,0.1,0.1,1);
  // floor
  glNormal3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_POLYGON);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-GROUND_WIDTH / 2.0f, 0.0f, GROUND_WIDTH / 2.0);
  
  glTexCoord2f(groundTexSize, 0.0f);
  glVertex3f(GROUND_WIDTH / 2.0f, 0.0f, GROUND_WIDTH / 2.0);
  
  glTexCoord2f(groundTexSize, groundTexSize);
  glVertex3f(GROUND_WIDTH / 2.0f, 0.0f, -GROUND_WIDTH / 2.0);
  
  glTexCoord2f(0.0f, groundTexSize);
  glVertex3f(-GROUND_WIDTH / 2.0f, 0.0f, -GROUND_WIDTH / 2.0);
  glEnd();

}

void drawSphere( void )
{
  glPushMatrix();
  glTranslatef(0,1,0);
	glColor4f(1,1,0,1);
  glutSolidSphere(2, 20, 20);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(4,0,0);
	glColor4f(0,0,0,1);
  glutSolidSphere(2, 20, 20);
  glPopMatrix();
}

void drawScreen( void )
{
  glBegin(GL_QUADS);
		glColor4f(1,1,1,1);
    glTexCoord2f(0,0);
    glVertex3f(-1.0, -1.0, -1);
    glTexCoord2f(1,0);
    glVertex3f(1.0, -1.0, -1);
    glTexCoord2f(1 ,1);
    glVertex3f(1.0, 1.0, -1);
    glTexCoord2f(0 ,1);
    glVertex3f(-1.0, 1.0, -1);
  glEnd();
}


/* Draw entire scene */
void drawScene( void )
{
  drawGround();
  drawSphere();
}

void drawDiffusePass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  updateView(gCameraPos, gLookAt);
  //glUseProgram(shShade);
  drawScene();
}

/* UPDATE FUNCTIONS - All functions updating the scene
 *----------------------------------------------------------------------------*/

/* Update scene elements */
void updateScene( float timestep )
{

}

/* Update current view */
void updateView( Vector eye, Vector lookAt)
{
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50,RENDER_WIDTH/RENDER_HEIGHT,10,400);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  // Update lookat
  gluLookAt(eye.x(), eye.y(), eye.z(),
            lookAt.x(), lookAt.y(), lookAt.z(),
            0, 1, 0);	
}

/* General Update function - called once per render */
void update( float timestep )
{
  updateScene( timestep );
  //updateLights( );
}


/* CALLBACK FUNCTIONS - General callback functions given to OpenGL
 *----------------------------------------------------------------------------*/

/* Display OpenGL callback */
void displayCallback( void )
{
  update( TIMESTEP );
	
  theta += 0.02;
  gCameraPos = Vector(sin(theta) * 13, 10, cos(theta) * 13);

	// geometry pass
	diffuseShader->enable();
	bloomFBOhelper.enable();
  drawDiffusePass();
	bloomFBOhelper.disable();
	
	// bloom pass
	bloomShader->enable();
	drawScreen();
	bloomShader->disable();
	
  glutSwapBuffers();
}

/* Keyboard OpenGL callback */
void keyboardCallback( unsigned char key, int x, int y )
{
  switch(key)
  {
    case 27: exit(0); break;
    //case 'o': case 'O': curPass = (++curPass % (MAX_PASS + 1)); break;
  }

  glutPostRedisplay();   
}

/* Resize OpenGL callback */
void reshapeCallback( int width, int height )
{
  // update global variables
  gWidth = width;
  gHeight = height;
  
  // rebuild viewport
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				
	glMatrixMode (GL_PROJECTION);										
	glLoadIdentity ();
	
	// rebuild perspective					
	gluPerspective (60.0f, (GLfloat)(width)/(GLfloat)(height),			
					0.1f, 500.0f);		
	glMatrixMode (GL_MODELVIEW);										
	glLoadIdentity ();			
}

/* Timer OpenGL callback */
void timerCallback( int step )
{
  displayCallback();
	glutTimerFunc( TIMESTEP,timerCallback, 0);
}


/* BUILDING FUNCTIONS - All functions that help build the scene
 *----------------------------------------------------------------------------*/

void initLights()
{
  /*gLighting.createLight(gLightPos.x(), gLightPos.y(), gLightPos.z(),
	                    0.8, 0.8, 0.8, 1,
	                    0.3,0.3,0.3,
	                    1.0, 1.0, 0.0);
*/
}

void initMaterials()
{
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess[1];
  
  ambient = {0.1, 0.1, 0.1, 1.0};
  diffuse = {0.9, 0.9, 0.9, 1.0};
  specular = {0.1, 0.1, 0.1, 1.0};
  shininess = {100.0};
  //gGroundMaterial = Material(ambient, diffuse, specular, shininess);

  ambient = {0.1, 0.1, 0.1, 1.0};
  diffuse = {0.9, 0.9, 0.9, 1.0};
  specular = {0.1, 0.1, 0.1, 1.0};
  shininess = {100.0};
  //gSphereMaterial = Material(ambient, diffuse, specular, shininess);
  
  /*
  ambient = {0.2, 0.1, 0.1, 1.0};
  diffuse = {0.8, 0.2, 0.2, 1.0};
  specular = {0.1, 0.1, 0.1, 1.0};
  shininess = {100.0};
  gRedMaterial = Material(ambient, diffuse, specular, shininess);
  */
}

bool initScene()
{
  initLights();
  initMaterials();
  bloomFBOhelper = FBOHelper();
  bool r = bloomFBOhelper.initialize( RENDER_WIDTH, RENDER_HEIGHT, 1 );
  bloomShader->setTextures(bloomFBOhelper.getTextureIDs(), 1);
  return r;
}

/* LOADING FUNCTIONS - All functions that load resources
 *----------------------------------------------------------------------------*/

/* Load texture and set to given ID*/
bool loadTexture( string filename, int textureID )
{
  LoadTexture( filename, textureID );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  return true;
}

/* Load all textures for the program */
bool loadTextures()
{
  //glGenTextures( 1, &diffuseTexture );
  //glGenTextures( 1, &randomTexture );
  //loadTexture( "resources/images/white.bmp", diffuseTexture );
  //loadTexture( "resources/images/random.bmp", randomTexture );
  //  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  //loadTexture( "resources/images/light.bmp", LIGHT_TEX );
  
  return true;
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

/* Load all shaders for the program */
bool loadShaders()
{
  bool success = true;
  getGLversion();
  
  //success &= installShader( "resources/shaders/texShade", &shTextureShade ); 
  diffuseShader = new DefaultShader();
  bloomShader = new PassShader();
  diffuseShader->load("resources/shaders/simpleShade"); 
  bloomShader->load("resources/shaders/bloom"); 
  //success &= installShader( "resources/shaders/ssao-depth", &shSSAODepth ); 
  //success &= installShader( "resources/shaders/ssao", &shSSAO ); 
  //success &= installShader( "resources/shaders/shadow", &shShadow );  
  //success &= installShader( "resources/shaders/depth", &shDepth ); 
  
  if(!success)
  {
    exit(-1);
  }
  
  return success;
}

/* INITIALIZTATION FUNCTIONS - main function as well as OpenGL initialization
 *----------------------------------------------------------------------------*/
/* Initialization function */
bool initialize()
{
  bool r = true;
  glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );
  glClearDepth( 1.0f );
  glDepthFunc( GL_LEQUAL );
  glEnable( GL_DEPTH_TEST );
  glShadeModel( GL_SMOOTH );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  
  return r;
}

/* Main Function */
int main( int argc, char *argv[] )
{
  // set default window size
  gWidth = RENDER_WIDTH;
  gHeight = RENDER_HEIGHT;

  gLightPos = Vector(5, 30, 0);

  gCameraPos = Vector(10,10,10);
  gLookAt = Vector(0,0,0);
  
  glutInit( &argc, argv );
  glutInitWindowPosition( 20,20 );
  glutInitWindowSize( gWidth, gHeight );
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutCreateWindow( "Final Project" );
  glutReshapeFunc( reshapeCallback );
  glutKeyboardFunc( keyboardCallback );
  glutDisplayFunc( displayCallback );
  glutTimerFunc(TIMESTEP, timerCallback, 0);

	glEnable(GL_CULL_FACE);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  
  //loadTextures();
  
  loadShaders();
  
  initialize();
  
  initScene();
	
  glutMainLoop();
  return 0;
}
