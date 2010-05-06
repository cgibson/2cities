#ifndef INCLUDES_H
#define INCLUDES_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <time.h>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

// constants
#define PI 3.141592654f

// enum for rectangle faces
//enum Face {top = 0, side1 = 1, side2 = 2, side3 = 3, side4 = 4, bottom = 5};

//very simple data structure to store 3d points
typedef struct Vector3
{
   float x;
   float y;
   float z;
   float vector_normal_x;
   float vector_normal_y;
   float vector_normal_z;
   Vector3(float in_x, float in_y, float in_z) 
      : x(in_x), y(in_y), z(in_z), vector_normal_x(0), vector_normal_y(0), vector_normal_z(0) {}
   Vector3() {}
}Vector3;

//data structure to store triangle - 
//note that v1, v2, and v3 are indexes into the vertex array
typedef struct Tri
{
   int v1;
   int v2;
   int v3;
	Vector3 center;
   Vector3 normal;
   Vector3 color;
   Tri(int in_v1, int in_v2, int in_v3) : v1(in_v1), v2(in_v2), v3(in_v3), normal(0, 1, 0){}
   Tri() : normal(0, 1, 0) {}
} Tri;

// struct to hold transform information
typedef struct transform_data
{
	char filename[20];	
	//double theta;
	Vector3 vectors[3];
	//GLfloat CRMarr[16];
	//double xpos, ypos, zpos;
	//double scaleFactor;
	int draw_solid;
	Vector3 center;
	float max_extent;
	transform_data() : draw_solid(0) {}
} transform_data;

/*typedef struct Timex
{
	int times_up;
	int function_parameter;
	task chore;
	bool expired;
	Timex (int in_times_up, task in_chore, int in_function_parameter) : times_up(in_times_up), chore(in_chore), function_parameter(in_function_parameter), expired(false) {}
	Timex() {}
}Timex;*/

// texture mapping data structures
/*data structure for the image used for  texture mapping */
typedef struct Image
{
	unsigned long sizeX;
  	unsigned long sizeY;
  	char *data;
} Image;

typedef struct Colour
{
	double red, green, blue;
	Colour (double in_red, double in_green, double in_blue) : 
			  red(in_red / 255), green(in_green / 255), blue(in_blue / 255) {}
	Colour() : red(0.0), green(0.0), blue(0.0) {}
}Colour;

typedef struct materialStruct
{
   GLfloat ambient[4];
   GLfloat diffuse[4];
   GLfloat specular[4];
   GLfloat shininess[1];
} materialStruct;

typedef struct intPoint
{
   int x, y, z;
	intPoint (int in_x, int in_y, int in_z) : x(in_x), y(in_y), z(in_z) {}
	intPoint() : x(0), y(0), z(0) {}
}intPoint;

typedef struct Point
{
	double x, y, z;
	Point (double in_x, double in_y, double in_z) : x(in_x), y(in_y), z(in_z) {}
	Point() : x(0.0), y(0.0), z(0.0) {}
}Point;

typedef struct SphericalCoordinate
{
	Point origin;	
	double rot_about_x, rot_about_z;
	SphericalCoordinate (Point in_origin, double in_rot_about_x, double in_rot_about_z) 
							  : origin(in_origin), rot_about_x(in_rot_about_x), rot_about_z(in_rot_about_z) {}
	SphericalCoordinate() {}
}SphericalCoordinate;

typedef struct Vector
{
	Point tail;
	Point head;   
   Vector(Point in_tail, Point in_head) 
      : tail(in_tail), head(in_head) {}
   Vector() {}
}Vector;

// texture methods
GLvoid LoadTexture(char* image_file, int texID);
static unsigned int getint(FILE *fp);
static unsigned int getshort(FILE *fp);
int ImageLoad(char *filename, Image *image);

// vector methods
Vector vector_plus_scalar(Vector A, double scalar);
Vector vector_minus_scalar(Vector A, double scalar);
Vector vector_times_scalar(Vector A, double scalar);
Vector vector_dividedBy_scalar(Vector A, double scalar);
bool point_on_plane(Point pt_in_question, Point pt_on_plane, Vector normal);
void draw_vector(Vector v);
Vector points_to_vector(Point A, Point B);
Vector normalize_vector(Vector v);
Vector polar_to_vector(Point tail, double theta, double phi, double radius);
void print_vector(Vector v);
double vector_magnitude(Vector v);
Vector crossProduct(Vector A, Vector B);
double dotProduct(Vector A, Vector B);

// point methods
void print_point(Point p);
void print_ipoint(intPoint p);
double distance_between(Point A, Point B);
Point polar_to_point(Point center, double theta, double phi, double radius);
Point point_hard_copy(Point p);
intPoint point_to_intPoint(Point p);
Point intpoint_to_Point(intPoint p);

// opengl methods
Point click2point(int x, int y);
float p2w_x(int xp, int GW, int GH);
float p2w_y(int yp, int GH);
void draw_xyz_axis();

// math methods
double rad2deg(double radians);
double deg2rad(double degrees);

#endif
