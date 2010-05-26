#ifndef _SKYSCRAPER_H_
#define _SKYSCRAPER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../helper/Vector.h"

class Skyscraper {
public:
    Skyscraper(int width, int height, Vector center, float offset);
    ~Skyscraper();

    void init();
    void draw();

    // property accessors
    int width() const { return _width; }
    int height() const { return _height; }
    Vector center() const { return _center; }
    Vector aabbMin() const { return _aabbMin; }
    Vector aabbMax() const { return _aabbMax; }

private:
	int _width;
	int _height;
	Vector _center;
	Vector _aabbMin;
	Vector _aabbMax;
	float _rand_offset;
	GLuint _displayList;

	static const int MIN_WIDTH;
	static const int MAX_WIDTH;
	static const int MIN_HEIGHT;
	static const int MAX_HEIGHT;
};

#endif
