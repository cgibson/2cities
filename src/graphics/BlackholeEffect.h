#ifndef _BLACKHOLEEFFECT_H_
#define _BLACKHOLEEFFECT_H_

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

#include "../system/global.h"
#include "../graphics/camera.h"
#include "../helper/Vector.h"
#include "../system/enum.h"
#include "Effect.h"

class BlackholeEffect : public Effect {
public:
    BlackholeEffect();
    ~BlackholeEffect();

    void init(int durationInMS);
    void update(int ms);
    void draw();

private:
};

#endif
