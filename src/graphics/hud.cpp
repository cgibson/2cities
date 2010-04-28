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
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(50.0, 50.0, 0.0);
        glVertex3f(global::width - 50.0, 50.0, 0.0);
        glVertex3f(50.0, global::height - 50.0, 0.0);
    glEnd();

    // reenable lighting and depth testing
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
