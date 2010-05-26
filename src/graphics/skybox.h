#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <vector>
#include <stdio.h>
#include <time.h>

#include "skyscraper.h"
#include "../helper/texloader.h"
#include "../helper/Vector.h"
#include "../system/global.h"
#include "Lighting.h"
#include "shader.h"

class Skybox {
public:
    Skybox();
    ~Skybox();

    void init();
    void update(int ms);
    void draw(float light_x, float light_y, float light_z); // pretty hacky

private:
	std::vector<Skyscraper *> _buildings;
	GLuint _circuit_tex;
	GLuint _sweeper_tex;
	GLuint _cloud_tex;
	GLuint _skyline_alpha;
	float _time;

	bool aabbCollides(Vector box1min, Vector box1max, Vector box2min, Vector box2max);

	static const float CITY_MIN_RADIUS;
	static const float CITY_MAX_RADIUS;
	static const int NUM_BUILDINGS;
	static const int BLDG_MIN_WIDTH;
	static const int BLDG_MAX_WIDTH;
	static const int BLDG_MIN_HEIGHT;
	static const int BLDG_MAX_HEIGHT;
	static const float SKY_SIZE;
	static const float SKY_HEIGHT;
	static const float SKY_WALL_OFFSET;
};

#endif
