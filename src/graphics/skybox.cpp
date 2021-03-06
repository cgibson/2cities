#include "skybox.h"
#include "graphics.h"

// handy macros for converting degrees to radians and back
#define DEG2RAD(x) ((x) * M_PI / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

const float Skybox::CITY_MIN_RADIUS = 110.0;
const float Skybox::CITY_MAX_RADIUS = 150.0;
const int Skybox::NUM_BUILDINGS = 150;
const int Skybox::BLDG_MIN_WIDTH = 4;
const int Skybox::BLDG_MAX_WIDTH = 12;
const int Skybox::BLDG_MIN_HEIGHT = 10;
const int Skybox::BLDG_MAX_HEIGHT = 20;
const float Skybox::SKY_SIZE = 1000.0;
const float Skybox::SKY_HEIGHT = 75.0;
const float Skybox::SKY_WALL_OFFSET = 600.0;

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
	_circuit_tex = texldr::loadBMP("resources/textures/circuit.bmp", NULL, NULL);
	_sweeper_tex = texldr::loadBMP("resources/textures/sweeper.bmp", NULL, NULL);
	_cloud_tex = texldr::loadBMP("resources/textures/clouds.bmp", NULL, NULL);
	_skyline_alpha = texldr::loadBMP("resources/textures/skylinealpha.bmp", NULL, NULL);

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

void Skybox::draw(float light_x, float light_y, float light_z, bool skyscrapers_only) // hackalicious!
{
	int loc;

	if (!skyscrapers_only)
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
		loc = glGetUniformLocation(gfx::skyShader.getProgram(), "cloud_tex");
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
	}

	if (!skyscrapers_only)
	{
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
	}
	else
	{
		gfx::simpleShader.enable();
		float boring[4] = {1.0, 1.0, 1.0, 1.0};
		Material mat(boring, boring, boring, boring);
        mat.applyMaterial(gfx::simpleShader, "");
	}

	// compute viewing frustum planes
	if (!skyscrapers_only) computeFrustum();

	// draw all skyscrapers
	for (int i = 0; i < NUM_BUILDINGS; i++)
	{
		if (!shouldCull(_buildings[i])) _buildings[i]->draw();
	}
}

bool Skybox::aabbCollides(Vector box1min, Vector box1max, Vector box2min, Vector box2max)
{
	if (box1min.x() > box2max.x() || box2min.x() > box1max.x()) return false;
	if (box1min.y() > box2max.y() || box2min.y() > box1max.y()) return false;
	if (box1min.z() > box2max.z() || box2min.z() > box1max.z()) return false;

    return true;
}

void Skybox::computeFrustum()
{
	float pmat[16];
    glGetFloatv(GL_PROJECTION_MATRIX, pmat);

    // extract left plane
    frustum[0].normal.x(pmat[3] + pmat[0]);
	frustum[0].normal.y(pmat[7] + pmat[4]);
	frustum[0].normal.z(pmat[11] + pmat[8]);
    frustum[0].dist = (pmat[15] + pmat[12]) / frustum[0].normal.mag();
    frustum[0].normal.norm();

    // extract right plane
    frustum[1].normal.x(pmat[3] - pmat[0]);
    frustum[1].normal.y(pmat[7] - pmat[4]);
    frustum[1].normal.z(pmat[11] - pmat[8]);
    frustum[1].dist = (pmat[15] - pmat[12]) / frustum[1].normal.mag();
    frustum[1].normal.norm();

    // extract near plane
    frustum[2].normal.x(pmat[3] + pmat[2]);
    frustum[2].normal.y(pmat[7] + pmat[6]);
    frustum[2].normal.z(pmat[11] + pmat[10]);
    frustum[2].dist = (pmat[15] + pmat[14]) / frustum[2].normal.mag();
    frustum[2].normal.norm();

    // extract far plane
    frustum[3].normal.x(pmat[3] - pmat[2]);
    frustum[3].normal.y(pmat[7] - pmat[6]);
    frustum[3].normal.z(pmat[11] - pmat[10]);
    frustum[3].dist = (pmat[15] - pmat[14]) / frustum[3].normal.mag();
    frustum[3].normal.norm();

    // extract bottom plane
    frustum[4].normal.x(pmat[3] + pmat[1]);
    frustum[4].normal.y(pmat[7] + pmat[5]);
    frustum[4].normal.z(pmat[11] + pmat[9]);
    frustum[4].dist = (pmat[15] + pmat[13]) / frustum[4].normal.mag();
    frustum[4].normal.norm();

    // extract top plane
    frustum[5].normal.x(pmat[3] - pmat[1]);
    frustum[5].normal.y(pmat[7] - pmat[5]);
    frustum[5].normal.z(pmat[11] - pmat[9]);
    frustum[5].dist = (pmat[15] - pmat[13]) / frustum[5].normal.mag();
    frustum[5].normal.norm();
}

bool Skybox::shouldCull(Skyscraper *skyscraper)
{
	int num_inside = 0;

    for (int i = 0; i < 6; i++)
    {
        float result;

        // check each corner of the bounding box
        result = skyscraper->aabbMin().x() * frustum[i].normal.x() +
                 skyscraper->aabbMin().y() * frustum[i].normal.y() +
                 skyscraper->aabbMin().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }

        result = skyscraper->aabbMin().x() * frustum[i].normal.x() +
                 skyscraper->aabbMin().y() * frustum[i].normal.y() +
                 skyscraper->aabbMax().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }

        result = skyscraper->aabbMin().x() * frustum[i].normal.x() +
                 skyscraper->aabbMax().y() * frustum[i].normal.y() +
                 skyscraper->aabbMin().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }

        result = skyscraper->aabbMin().x() * frustum[i].normal.x() +
                 skyscraper->aabbMax().y() * frustum[i].normal.y() +
                 skyscraper->aabbMax().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }

        result = skyscraper->aabbMax().x() * frustum[i].normal.x() +
                 skyscraper->aabbMin().y() * frustum[i].normal.y() +
                 skyscraper->aabbMin().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }

        result = skyscraper->aabbMax().x() * frustum[i].normal.x() +
                 skyscraper->aabbMin().y() * frustum[i].normal.y() +
                 skyscraper->aabbMax().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }

        result = skyscraper->aabbMax().x() * frustum[i].normal.x() +
                 skyscraper->aabbMax().y() * frustum[i].normal.y() +
                 skyscraper->aabbMin().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }

        result = skyscraper->aabbMax().x() * frustum[i].normal.x() +
                 skyscraper->aabbMax().y() * frustum[i].normal.y() +
                 skyscraper->aabbMax().z() * frustum[i].normal.z() +
                 frustum[i].dist;
        if (result > 0)
        {
            num_inside++;
            continue;
        }
    }

    if (num_inside == 6) return false;

    return true;
}

#undef DEG2RAD
#undef RAD2DEG
