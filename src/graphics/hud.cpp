#include "hud.h"

Hud::Hud()
{

}

void Hud::draw()
{
    // switch to a 2D projection in pixel space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, global::width, 0.0, global::height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // disable lighting and depth testing
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // for now just draw a red rectangle in the middle of the screen
    glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(global::width / 4.0, global::height / 4.0);
        glVertex2f(global::width / 4.0, global::height * 3.0 / 4.0);
        glVertex2f(global::width * 3.0 / 4.0, global::height * 3.0 / 4.0);
        glVertex2f(global::width * 3.0 / 4.0, global::height);
    glEnd();
}
