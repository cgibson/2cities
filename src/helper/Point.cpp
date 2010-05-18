#include "Point.h"

Point::Point(GLdouble x, GLdouble y, GLdouble z)
{
  p_x = x;
  p_y = y;
  p_z = z;
}

Point::Point(GLdouble clicked_x, GLdouble clicked_y)
{
	GLint viewport[4];
	GLdouble modelview_matrix[16];
	GLdouble projection_matrix[16];

   GLdouble *mvMatrix = (GLdouble*)modelview_matrix;
   GLdouble *pMatrix = (GLdouble*)projection_matrix;
   GLint *vMatrix = (GLint*)viewport;

	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	//glGetDoublev( GL_MODELVIEW_MATRIX, modelview_matrix );
	//glGetDoublev( GL_PROJECTION_MATRIX, projection_matrix );
	
   gfx::renderer.getMatrices(&mvMatrix, &pMatrix, &vMatrix);

   //glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)clicked_x;
	winY = (float)viewport[3] - (float)clicked_y;
	glReadPixels( clicked_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview_matrix, projection_matrix, viewport, &posX, &posY, &posZ);
	
	printf("winX = %f, winY = %f, winZ = %f\n", winX, winY, winZ);
	int i, j;	
	printf("modelview:\n");	
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%f ", modelview_matrix[i]);
			i++;		
		}		
		printf("\n");
	}
	printf("\nprojection:\n");	
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%f ", projection_matrix[i]);
			i++;		
		}
		printf("\n");
	}
	printf("\n");
   printf("viewport:\n");
   for(j = 0; j < 4; j++)
	{
		printf("%d ", viewport[j]);
		i++;		
	}	
   printf("\n");

	p_x = posX;
	p_y = posY;
	p_z = posZ;
}

Point Point::operator=( const Point& p1) {
	this->p_x = p1.p_x;
	this->p_y = p1.p_y;
	this->p_z = p1.p_z;
	return(*this );
}

Point::Point( const Point& p )
{
  p_x = p.p_x;
  p_y = p.p_y;
  p_z = p.p_z;
}

Point::Point()
{
  p_x = 0;
  p_y = 0;
  p_z = 0;
}

void Point::adjustPointForBlocksize(int blocksize)
{
	// adjust click for blocksize
	if(p_z < 0)
		p_z = ( p_z - ((int)p_z % -blocksize) );
	else
		p_z = ( p_z - ((int)p_z % blocksize) );			
	if(p_x < 0)
		p_x = ( p_x - ((int)p_x % -blocksize) );
	else
		p_x = ( p_x - ((int)p_x % blocksize) );
}

char* Point::str( void ) {
    char *buffer = (char*)calloc(300, sizeof(char));
    sprintf(buffer, "(%.2f, %.2f, %.2f)", p_x, p_y, p_z);
    return buffer;
}

GLdouble Point::getx()
{
  return p_x;
}

GLdouble Point::gety()
{
  return p_y;
}

GLdouble Point::getz()
{
  return p_z;
}
