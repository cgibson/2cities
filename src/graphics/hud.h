#ifndef _HUD_H_
#define _HUH_H_

#include <GL/glut.h>
#include "../system/global.h"

class Hud {
public:
    Hud();

    // this function should be called last in the rendering pipeline because
    // it OBLITERATES the 3D camera projection
    void draw();

private:

};

#endif
