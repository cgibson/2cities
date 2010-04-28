#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "OGLFT.h"

OGLFT::Translucent *bigFont;
OGLFT::Translucent *smallFont;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    bigFont->draw(50, 50, "Hello, OGLFT!");
    smallFont->draw(50, 20, "Hello, smaller monospaced font!");

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("OGLFT Font Rendering Test");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    bigFont = new OGLFT::Translucent("../../resources/fonts/sui_generis_free.ttf", 48);
    if (!bigFont->isValid())
    {
        fprintf(stderr, "Failed to open font face.\n");
    }
    bigFont->setForegroundColor(1.0, 1.0, 1.0, 1.0);
    
    smallFont = new OGLFT::Translucent("../../resources/fonts/consolas.ttf", 12);
    if (!smallFont->isValid())
    {
        fprintf(stderr, "Failed to open font face.\n");
    }
    smallFont->setForegroundColor(1.0, 1.0, 1.0, 1.0);

    glutMainLoop();

    return EXIT_SUCCESS;
}
