#include "skyscraper.h"
#include "graphics.h"

int Skyscraper::displayList = -1;

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
	// generate the display list if we haven't already
	if (displayList < 0)
	{
		displayList = glGenLists(1);

		if(!displayList)
		{
			fprintf(stderr, "Error: unable to generate skyscraper display list\n");
			exit(EXIT_FAILURE);
		}

		// compile the display list
		glNewList(displayList, GL_COMPILE);
			glBegin(GL_QUADS);
				// front face
				glNormal3f(0.0, 0.0, 1.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-0.5, 0.0, 0.5);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.5, 0.0, 0.5);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(0.5, 1.0, 0.5);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(-0.5,  1.0, 0.5);

				// back face
				glNormal3f(0.0, 0.0, -1.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.5, 0.0, -0.5);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-0.5, 0.0, -0.5);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-0.5, 1.0, -0.5);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.5,  1.0, -0.5);

				// left face
				glNormal3f(-1.0, 0.0, 0.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-0.5, 0.0, -0.5);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-0.5, 0.0, 0.5);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-0.5, 1.0, 0.5);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(-0.5, 1.0, -0.5);

				// right face
				glNormal3f(1.0, 0.0, 0.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.5, 0.0, 0.5);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.5, 0.0, -0.5);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(0.5, 1.0, -0.5);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.5, 1.0, 0.5);

				// top face
				glNormal3f(0.0, 1.0, 0.0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-0.5, 1.0, 0.5);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.5, 1.0, 0.5);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(0.5, 1.0, -0.5);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(-0.5, 1.0, -0.5);

				// bottom face
				// don't bother (can't see it!)

			glEnd();
		glEndList();
	}
}

void Skyscraper::draw()
{
	// set shader uniforms
	int loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "rand_offset");
	glUniform1f(loc, _rand_offset);

	glPushMatrix();
		glTranslatef(_center.x(), 0.0, _center.z());
		glScalef(_width, _height, _width);
		glCallList(displayList);
	glPopMatrix();
}
