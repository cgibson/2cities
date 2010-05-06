#include "includes.h"

Point click2point(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview_matrix[16];
	GLdouble projection_matrix[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview_matrix );
	glGetDoublev( GL_PROJECTION_MATRIX, projection_matrix );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview_matrix, projection_matrix, viewport, &posX, &posY, &posZ);

	return Point(posX, posY, posZ);
}

/*debugging routine which just draws the vertices of the mesh
void DrawAllVerts()
{
   glColor3f(1.0, 0.0, 1.0);
   glBegin(GL_POINTS);
   for (unsigned int k = 0; k < Vertices.size(); k++)
   {
      glVertex3f(Vertices[k]->x, Vertices[k]->y, Vertices[k]->z);
   }
   glEnd();
}*/

// texture methods!
//routines to load in a bmp files - must be 2^nx2^m and a 24bit bmp
GLvoid LoadTexture(char* image_file, int texID) 
{ 
	Image *TextureImage = (Image *) malloc(sizeof(Image));
  	if (TextureImage == NULL) 
	{
   	printf("Error allocating space for image");
    	exit(1);
  	}
	printf("trying to load %s\n", image_file);
  	//cout << "trying to load " << image_file << endl;
  	if (!ImageLoad(image_file, TextureImage)) 
	{
   	exit(1);
  	}  
  	/*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
  	/*  x size from image, y size from image,                                              */    
  	/*  border 0 (normal), rgb color data, unsigned byte data, data  */ 
  	glBindTexture(GL_TEXTURE_2D, texID);
  	glTexImage2D(GL_TEXTURE_2D, 0, 3,
   				 TextureImage->sizeX, TextureImage->sizeY,
    				 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /*  cheap scaling when image bigger than texture */    
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /*  cheap scaling when image smalled than texture*/
}

/* BMP file loader loads a 24-bit bmp file only */
/*
* getint and getshort are help functions to load the bitmap byte by byte
*/
static unsigned int getint(FILE *fp) 
{
	int c, c1, c2, c3;
  
  	/*  get 4 bytes */ 
  	c = getc(fp);  
  	c1 = getc(fp);  
  	c2 = getc(fp);  
  	c3 = getc(fp);
  
  	return ((unsigned int) c) +   
    		(((unsigned int) c1) << 8) + 
    		(((unsigned int) c2) << 16) +
    		(((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp)
{
	int c, c1;
  
  	/* get 2 bytes*/
  	c = getc(fp);  
  	c1 = getc(fp);
  
  	return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

/*  quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.  */
int ImageLoad(char *filename, Image *image) 
{
	FILE *file;
  	unsigned long size;                 /*  size of the image in bytes. */
  	unsigned long i;                    /*  standard counter. */
  	unsigned short int planes;          /*  number of planes in image (must be 1)  */
  	unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
  	char temp;                          /*  used to convert bgr to rgb color. */
  
  	/*  make sure the file is there. */
  	if ((file = fopen(filename, "rb")) == NULL) 
	{
   	printf("File Not Found : %s\n", filename);
    	return 0;
  	}
  
  	/*  seek through the bmp header, up to the width height: */
  	fseek(file, 18, SEEK_CUR);
  
  	/*  No 100% errorchecking anymore!!! */
  
  	/*  read the width */
	image->sizeX = getint (file);
  
  	/*  read the height */ 
  	image->sizeY = getint (file);
  
  	/*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
  	size = image->sizeX * image->sizeY * 3;
  
  	/*  read the planes */    
  	planes = getshort(file);
  	if (planes != 1) 
	{
   	printf("Planes from %s is not 1: %u\n", filename, planes);
    	return 0;
  	}
  
  	/*  read the bpp */    
  	bpp = getshort(file);
  	if (bpp != 24) 
	{
   	printf("Bpp from %s is not 24: %u\n", filename, bpp);
    	return 0;
  	}
  
  	/*  seek past the rest of the bitmap header. */
  	fseek(file, 24, SEEK_CUR);
  
  	/*  read the data.  */
  	image->data = (char *) malloc(size);
  	if (image->data == NULL) 
	{
    	printf("Error allocating memory for color-corrected image data");
    	return 0; 
  	}
  
  	if ((i = fread(image->data, size, 1, file)) != 1) 
	{
    	printf("Error reading image data from %s.\n", filename);
    	return 0;
  	}
   
	/*  reverse all of the colors. (bgr -> rgb) */
  	for (i = 0; i < size; i += 3) 
	{ 
    	temp = image->data[i];
    	image->data[i] = image->data[i + 2];
    	image->data[i + 2] = temp;
  	}
  
  	fclose(file); /* Close the file and release the filedes */
  
  	/*  we're done. */
  	return 1;
}

void draw_vector(Vector v)
{
	glBegin(GL_LINES);
	glVertex3f(v.head.x, v.head.y, v.head.z);
	glVertex3f(v.tail.x, v.tail.y, v.tail.z);
	glEnd();
}

Point intpoint_to_Point(intPoint p)
{
   return Point(p.x, p.y, p.z);
}

intPoint point_to_intPoint(Point p)
{
   return intPoint((int)p.x, (int)p.y, (int)p.z);
}

void print_point(Point p)
{
	printf("xyz: (%f, %f, %f)\n", p.x, p.y, p.z);
}

void print_ipoint(intPoint p)
{
	printf("xyz: (%d, %d, %d)\n", p.x, p.y, p.z);
}

void print_vector(Vector v)
{
	printf("tail: (%f, %f, %f)\nhead: (%f, %f, %f)\n", v.tail.x, v.tail.y, v.tail.z, v.head.x, v.head.y, v.head.z);
}

// this method is so screwed up!
Point polar_to_point(Point center, double theta, double phi, double radius)
{
	Point p;
	//p.x = radius * sin(deg2rad(theta)) * cos(deg2rad(phi)) + center.x;
	//p.y = radius * sin(deg2rad(theta)) * sin(deg2rad(phi)) + center.y;
	//p.z = radius * cos(deg2rad(theta)) + center.z;
	p.x = radius * sin(deg2rad(phi)) * sin(deg2rad(theta)) + center.x;
	p.y = radius * cos(deg2rad(phi)) + center.y;
	p.z = radius * sin(deg2rad(phi)) * cos(deg2rad(theta)) + center.z;
	return p;
}

Point point_hard_copy(Point p)
{
	return Point(p.x, p.y, p.z);
}

// takes the arguments: a polar coordinate and a vector tail, and returns a vector
Vector polar_to_vector(Point tail, double theta, double phi, double radius)
{
	Vector v;
	// set vector tail
	v.tail.x = tail.x;
	v.tail.y = tail.y;
	v.tail.z = tail.z;
	// set vector head - using the tail of the vector as an offset from (0, 0, 0)
	v.head.x = radius * sin(deg2rad(phi)) * sin(deg2rad(theta)) + v.tail.x;
	v.head.y = radius * cos(deg2rad(phi)) + v.tail.y;
	v.head.z = radius * sin(deg2rad(phi)) * cos(deg2rad(theta)) + v.tail.z;
	//v.head.x = radius * sin(deg2rad(theta)) * cos(deg2rad(phi)) + v.tail.x;
	//v.head.y = radius * sin(deg2rad(theta)) * sin(deg2rad(phi)) + v.tail.y;
	//v.head.z = radius * cos(deg2rad(theta)) + v.tail.z;
	//v.head.x = radius * cos(deg2rad(phi)) * cos(deg2rad(theta)) + v.tail.x;
	//v.head.y = radius * sin(deg2rad(phi)) + v.tail.y;
	//v.head.z = radius * cos(deg2rad(phi)) * cos(deg2rad(90 - theta)) + v.tail.z;
	return v;
}

// given two points, returns a vector between these points
Vector points_to_vector(Point tail, Point head)
{
   Vector v;
   // set vector tail
	v.tail.x = tail.x;
	v.tail.y = tail.y;
	v.tail.z = tail.z;
	// set vector head
	v.head.x = head.x;
	v.head.y = head.y;
	v.head.z = head.z;
   return v;
}

// compares the difference between the HEAD of two vectors (ignoring their tails)
double distance_between(Point A, Point B)
{
	double d = sqrt( (A.x - B.x) * (A.x - B.x) + 
                    (A.y - B.y) * (A.y - B.y) + 
                    (A.z - B.z) * (A.z - B.z) );
	return d;
}

// divide the head by the distance between the tail and head
Vector normalize_vector(Vector v)
{
   double magnitude;
	// bring points back to origin
	v.head.x += (0 - v.tail.x);
	v.head.y += (0 - v.tail.y);
	v.head.z +=	(0 - v.tail.z);
	v.tail = Point();

	// normalize
   magnitude = vector_magnitude(v);
   v.head.x = v.head.x / magnitude;   
   v.head.y = v.head.y / magnitude;
   v.head.z = v.head.z / magnitude;
   return v;
}

double vector_magnitude(Vector v)
{
	return sqrt( (v.head.x - v.tail.x) * (v.head.x - v.tail.x) + 
					 (v.head.y - v.tail.y) * (v.head.y - v.tail.y) + 
					 (v.head.z - v.tail.z) * (v.head.z - v.tail.z) );
}

Vector crossProduct(Vector A, Vector B)
{
	Vector product;
	// vector - applies only to vectors from the center
	product.head.x = A.head.y * B.head.z - A.head.z * B.head.y;
	product.head.y = A.head.z * B.head.x - A.head.x * B.head.z;
	product.head.z = A.head.x * B.head.y - A.head.y * B.head.x;
	// set vector tail
	product.tail.x = A.tail.x;
	product.tail.y = A.tail.y;
	product.tail.z = A.tail.z;
	
	return product;
}

double dotProduct(Vector A, Vector B)
{
	double product;
	product = A.head.x * B.head.x + A.head.y * B.head.y + A.head.z * B.head.z;
	return product;
}

double rad2deg(double radians)
{
   return radians * 180.0 / 3.1415926;
}

double deg2rad(double degrees)
{
   return degrees * PI / 180.0;
}

/*bool point_on_plane(Point pt_in_question, Point pt_on_plane, Vector normal)
{
	if(normal.x * (pt_in_question.x - pt_on_plane.x) + 
		normal.y * (pt_in_question.y - pt_on_plane.y) + 
		normal.z * (pt_in_question.z - pt_on_plane.z) )
		return true;
	else
		return false;
}*/

Vector vector_plus_scalar(Vector A, double scalar)
{
	A.head.x = A.head.x + scalar;
	A.head.y = A.head.y + scalar;
	A.head.z = A.head.z + scalar;
	return A;
}

Vector vector_minus_scalar(Vector A, double scalar)
{
	A.head.x = A.head.x - scalar;
	A.head.y = A.head.y - scalar;
	A.head.z = A.head.z - scalar;
	return A;
}

Vector vector_times_scalar(Vector A, double scalar)
{
	A.head.x = A.head.x * scalar;
	A.head.y = A.head.y * scalar;
	A.head.z = A.head.z * scalar;
	return A;
}

Vector vector_dividedBy_scalar(Vector A, double scalar)
{
	A.head.x = A.head.x / scalar;
	A.head.y = A.head.y / scalar;
	A.head.z = A.head.z / scalar;
	return A;
}

void draw_xyz_axis()
{
	//glDisable(GL_LIGHT0);  
	glBegin(GL_LINES);
   
   glColor3f(122 / (float)255, 122 / (float)255, 122 / (float)255);     // neg (grey) 
   glVertex3f(0, 0, 0);
   glVertex3f(-150, 0, 0);
   
   glVertex3f(0, 0, 0);
   glVertex3f(0, -150, 0);
   
   glVertex3f(0, 0, 0);
   glVertex3f(0, 0, -150);
   
   glColor3f(0, 1, 0);     //pos z (green)
   glVertex3f(0, 0, 0);
   glVertex3f(0, 0, 150);
   
   glColor3f(1, 0, 0);     //pos x (red)
   glVertex3f(0, 0, 0);
   glVertex3f(150, 0, 0);
   
   glColor3f(0, 0, 1);     //pos y (blue)
   glVertex3f(0, 0, 0);
   glVertex3f(0, 150, 0);

   glEnd();
	//glEnable(GL_LIGHT0);
}

//return x world coord
float p2w_x(int xp, int GW, int GH)
{
	float xi;	//image coord
	float xw;	//world coord

	xi = (2*xp + 1 - GW) / (float)GW;

	xw = xi * GW / (float)GH;

	return xw;
}

//reurn y world coord
float p2w_y(int yp, int GH)
{
	float yi;	//image coord
	float yw;	//world coord

	yi = (2*yp + 1 - GH) / (float)GH;

	yw = yi;

	return yw;
}
