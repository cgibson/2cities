#include <GL/glut.h>
#include "../../src/helper/texloader.h"

int gw, gh;
unsigned int tex;

void drawCube()
{
    glBindTexture(GL_TEXTURE_2D, tex);

    // front face
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-0.5, -0.5, 0.5);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(0.5, -0.5, 0.5);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // right face
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(0.5, -0.5, 0.5);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(0.5, -0.5, -0.5);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(0.5, 0.5, 0.5);
    glEnd();

        // top face
    glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-0.5, 0.5, 0.5);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-0.5, 0.5, -0.5);
    glEnd();

    // no more because i'm lazy
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(1.5, 1.5, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    drawCube();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    gw = w;
    gh = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)gw / gh, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, gw, gh);

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    gw = 800;
    gh = 600;

    glutInit(&argc, argv);
    glutInitWindowSize(gw, gh);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Texture Loader Test");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    tex = texldr::loadBMP("../../resources/textures/circuit1layer1.bmp");

    glutMainLoop();

    return 0;
}
