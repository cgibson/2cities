#include "skyscraper.h"
#include "graphics.h"

const int Skyscraper::MIN_WIDTH = 5;
const int Skyscraper::MAX_WIDTH = 10;
const int Skyscraper::MIN_HEIGHT = 5;
const int Skyscraper::MAX_HEIGHT = 10;

Skyscraper::Skyscraper(int width, int height, Vector center, float offset)
{
	_width = width;
	_height = height;
	_center = center;
	_rand_offset = offset;

	// compute the aabb for this skyscraper
	_aabbMin.x(_center.x() - (_width / 2.0));
	_aabbMin.y(_center.y());
	_aabbMin.z(_center.z() - (_width / 2.0));
	_aabbMax.x(_center.x() + (_width / 2.0));
	_aabbMax.y(_center.y() + _height);
	_aabbMax.z(_center.z() + (_width / 2.0));
}

Skyscraper::~Skyscraper()
{
    // nothing yet
}

void Skyscraper::init()
{
	// compile a display list for the building's geometry
	_displayList = glGenLists(1);

	if(!_displayList)
	{
		fprintf(stderr, "Error: unable to generate skyscraper display list\n");
		exit(EXIT_FAILURE);
	}

	// shorthand because it makes things easier
	float b = _width / 2.0;
	//float w = _width;
	float h = _height;
	float s = 1.0;
	float t = 1.0;

	// compile the display list
	glNewList(_displayList, GL_COMPILE);
		glBegin(GL_QUADS);
			// front face
			glNormal3f(0.0, 0.0, 1.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-b, 0.0, b);
			glTexCoord2f(s, 0.0);
			glVertex3f(b, 0.0, b);
			glTexCoord2f(s, t);
			glVertex3f(b, h, b);
			glTexCoord2f(0.0, t);
			glVertex3f(-b,  h, b);

			// back face
			glNormal3f(0.0, 0.0, -1.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(b, 0.0, -b);
			glTexCoord2f(s, 0.0);
			glVertex3f(-b, 0.0, -b);
			glTexCoord2f(s, t);
			glVertex3f(-b, h, -b);
			glTexCoord2f(0.0, t);
			glVertex3f(b,  h, -b);

			// left face
			glNormal3f(-1.0, 0.0, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-b, 0.0, -b);
			glTexCoord2f(s, 0.0);
			glVertex3f(-b, 0.0,  b);
			glTexCoord2f(s, t);
			glVertex3f(-b, h,  b);
			glTexCoord2f(0.0, t);
			glVertex3f(-b, h, -b);

			// right face
			glNormal3f(1.0, 0.0, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(b, 0.0, b);
			glTexCoord2f(s, 0.0);
			glVertex3f(b, 0.0, -b);
			glTexCoord2f(s, t);
			glVertex3f(b, h, -b);
			glTexCoord2f(0.0, t);
			glVertex3f(b, h, b);

			// top face
			glNormal3f(0.0, 1.0, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-b, h, b);
			glTexCoord2f(s, 0.0);
			glVertex3f(b, h, b);
			glTexCoord2f(s, t);
			glVertex3f(b, h, -b);
			glTexCoord2f(0.0, t);
			glVertex3f(-b, h, -b);

			// bottom face
			// don't bother (can't see it!)

		glEnd();
	glEndList();
}

void Skyscraper::draw()
{
	// set shader uniforms
	int loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "rand_offset");
	glUniform1f(loc, _rand_offset);

	glPushMatrix();
		glTranslatef(_center.x(), 0.0, _center.z());
		glCallList(_displayList);
	glPopMatrix();
}
