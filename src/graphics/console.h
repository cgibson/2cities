#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <list>
#include <GL/glut.h>
#include "../system/global.h"
#include "OGLFT.h"

#define CONSOLE_WIDTH 400
#define CONSOLE_HEIGHT 400
#define CONSOLE_TOP 10
#define CONSOLE_LEFT 10

class Console {
public:
    Console();

    void init();
    void draw();
    void update(int ms);

private:
    
};

#endif
