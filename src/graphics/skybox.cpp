#include "skybox.h"
#include "graphics.h"

// handy macros for converting degrees to radians and back
#define DEG2RAD(x) ((x) * M_PI / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

const float Skybox::CITY_MIN_RADIUS = 80.0;
const float Skybox::CITY_MAX_RADIUS = 125.0;
const int Skybox::NUM_BUILDINGS = 150;
const int Skybox::BLDG_MIN_WIDTH = 4;
const int Skybox::BLDG_MAX_WIDTH = 12;
const int Skybox::BLDG_MIN_HEIGHT = 10;
const int Skybox::BLDG_MAX_HEIGHT = 20;
const float Skybox::SKY_SIZE = 1000.0;
const float Skybox::SKY_HEIGHT = 75.0;
const float Skybox::SKY_WALL_OFFSET = 700.0;

Skybox::Skybox()
{
	_circuit_tex = 0;
	_sweeper_tex = 0;
	_time = 0.0;
}

Skybox::~Skybox()
{
    _buildings.clear();
}

void Skybox::init()
{
	// load necessary textures
	_circuit_tex = texldr::loadBMP("resources/textures/circuit.bmp");
	_sweeper_tex = texldr::loadBMP("resources/textures/sweeper.bmp");
	_cloud_tex = texldr::loadBMP("resources/textures/clouds.bmp");
	_skyline_alpha = texldr::loadBMP("resources/textures/skylinealpha.bmp");

	// randomize the madness!
	srand48(time(NULL));

	// generate buildings
	for (int i = 0; i < NUM_BUILDINGS; i++)
	{
		int width = (int)floor(drand48() * (BLDG_MAX_WIDTH - BLDG_MIN_WIDTH)) + BLDG_MIN_WIDTH;
		int height = (int)floor(drand48() * (BLDG_MAX_HEIGHT - BLDG_MIN_HEIGHT)) + BLDG_MIN_HEIGHT;

		Skyscraper *building;
		bool success = false;
		while (!success)
		{
			// pick a random theta and radius
			float theta = drand48() * 360.0;
			float radius = (drand48() * (CITY_MAX_RADIUS - CITY_MIN_RADIUS)) + CITY_MIN_RADIUS;

			// convert from polar to cartesian coords
			Vector center(radius * cosf(DEG2RAD(theta)), 0.0, radius * sinf(DEG2RAD(theta)));

			// create the new building
			building = new Skyscraper(width, height, center, drand48() * 100.0);

			// does it intersect with any of the other buildings?
			bool intersects = false;
			for (int i = 0; i < (int)_buildings.size(); i++)
			{
				if (aabbCollides(building->aabbMin(), building->aabbMax(), _buildings[i]->aabbMin(), _buildings[i]->aabbMax()))
				{
					intersects = true;
					break;
				}
			}

			if (intersects)
			{
				delete building;
			}
			else
			{
				success = true;
			}
		}

		_buildings.push_back(building);
	}

	for (int i = 0; i < NUM_BUILDINGS; i++)
	{
		_buildings[i]->init();
	}
}

void Skybox::update(int ms)
{
	// update our time accumulator
	_time += ms / 1000.0;
}

void Skybox::draw(float light_x, float light_y, float light_z) // hackalicious!
{
	// bind texture units for the textures we're going to use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _circuit_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _sweeper_tex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _cloud_tex);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _skyline_alpha);

	// switch to sky shader
	gfx::skyShader.enable();

	// set fixed uniforms
	int loc = glGetUniformLocation(gfx::skyShader.getProgram(), "cloud_tex");
	glUniform1i(loc, 2); // tex unit 2
	loc = glGetUniformLocation(gfx::skyShader.getProgram(), "time");
	glUniform1f(loc, _time);

	// draw the sky plane
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-SKY_SIZE / 2.0, SKY_HEIGHT, -SKY_SIZE / 2.0);
		glTexCoord2f(4.0, 0.0);
		glVertex3f(SKY_SIZE / 2.0, SKY_HEIGHT, -SKY_SIZE / 2.0);
		glTexCoord2f(4.0, 4.0);
		glVertex3f(SKY_SIZE / 2.0, SKY_HEIGHT, SKY_SIZE / 2.0);
		glTexCoord2f(0.0, 4.0);
		glVertex3f(-SKY_SIZE / 2.0, SKY_HEIGHT, SKY_SIZE / 2.0);
	glEnd();

	// switch to the distant shader
	gfx::distantShader.enable();	
	// set fixed uniforms
	loc = glGetUniformLocation(gfx::distantShader.getProgram(), "circuit_tex");
	glUniform1i(loc, 0); // tex unit 0
	loc = glGetUniformLocation(gfx::distantShader.getProgram(), "sweeper_tex");
	glUniform1i(loc, 1); // tex unit 1
	loc = glGetUniformLocation(gfx::distantShader.getProgram(), "skyline_alpha");
	glUniform1i(loc, 3); // tex unit 3
	loc = glGetUniformLocation(gfx::distantShader.getProgram(), "time");
	glUniform1f(loc, _time);
	
	// draw the wall planes
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 0.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 1.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 0.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 1.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 0.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 1.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f((SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
	glEnd();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 0.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, 0.0, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(4.0, 1.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, (SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-(SKY_SIZE - SKY_WALL_OFFSET) / 2.0, SKY_HEIGHT, -(SKY_SIZE - SKY_WALL_OFFSET) / 2.0);
	glEnd();
	

	// switch to the skyscraper shader
	gfx::skyscraperShader.enable();	

	// set fixed uniforms for all buildings
	loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "amb_contrib");
	glUniform1f(loc, 0.2);
	loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "diff_contrib");
	glUniform1f(loc, 0.8);
	loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "min_glow");
	glUniform1f(loc, 0.2);
	loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "max_glow");
	glUniform1f(loc, 1.0);
	loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "time");
	glUniform1f(loc, _time);
	loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "light_pos");
	glUniform3f(loc, light_x, light_y, light_z);
	loc = glGetUniformLocation(gfx::skyscraperShader.getProgram(), "eye_pos");
	glUniform3f(loc, global::camera->position().x(), global::camera->position().y(), global::camera->position().z());

	// draw all skyscrapers
	for (int i = 0; i < NUM_BUILDINGS; i++)
	{
		_buildings[i]->draw();
	}
}

bool Skybox::aabbCollides(Vector box1min, Vector box1max, Vector box2min, Vector box2max)
{
	if (box1min.x() > box2max.x() || box2min.x() > box1max.x()) return false;
	if (box1min.y() > box2max.y() || box2min.y() > box1max.y()) return false;
	if (box1min.z() > box2max.z() || box2min.z() > box1max.z()) return false;

    return true;
}

#undef DEG2RAD
#undef RAD2DEG
