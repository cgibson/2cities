#include "skybox.h"
#include "graphics.h"

// handy macros for converting degrees to radians and back
#define DEG2RAD(x) ((x) * M_PI / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

const float Skybox::CITY_MIN_RADIUS = 75.0;
const float Skybox::CITY_MAX_RADIUS = 100.0;
const int Skybox::NUM_BUILDINGS = 75;
const int Skybox::BLDG_MIN_WIDTH = 4;
const int Skybox::BLDG_MAX_WIDTH = 12;
const int Skybox::BLDG_MIN_HEIGHT = 10;
const int Skybox::BLDG_MAX_HEIGHT = 20;

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

	// call all skyscraper updates
	for (int i = 0; i < NUM_BUILDINGS; i++)
	{
		_buildings[i]->update(_time);
	}
}

void Skybox::draw(float light_x, float light_y, float light_z) // hackalicious!
{
	// bind circuit and sweeper textures to units 0 and 1 respectively
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _circuit_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _sweeper_tex);

	// set fixed uniforms for all buildings
	int loc = glGetUniformLocation(gfx::shCircuity, "circuit_tex");
	glUniform1i(loc, 0); // tex unit 0
	loc = glGetUniformLocation(gfx::shCircuity, "sweeper_tex");
	glUniform1i(loc, 1); // tex unit 1
	loc = glGetUniformLocation(gfx::shCircuity, "amb_contrib");
	glUniform1f(loc, 0.3);
	loc = glGetUniformLocation(gfx::shCircuity, "diff_contrib");
	glUniform1f(loc, 0.7);
	loc = glGetUniformLocation(gfx::shCircuity, "edge_strength");
	glUniform1f(loc, 0.2);
	loc = glGetUniformLocation(gfx::shCircuity, "light_pos");
	glUniform3f(loc, light_x, light_y, light_z);
	loc = glGetUniformLocation(gfx::shCircuity, "eye_pos");
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
