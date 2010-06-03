#ifndef _SHOCKWAVEEFFECT_H_
#define _SHOCKWAVEEFFECT_H_

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

#include "../system/enum.h"
#include "Effect.h"

class ShockwaveEffect : public Effect {
public:
    ShockwaveEffect();
    ~ShockwaveEffect();

    void init(int durationInMS, float maxSize);
    void update(int ms);
    void draw();

private:
	float _maxSize;
};

#endif