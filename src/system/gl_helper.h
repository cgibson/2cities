#include <GL/gl.h>
#include <GL/glut.h>
#include "global.h"

#ifndef _GL_HELPER_H_
#define _GL_HELPER_H_

namespace gl{
  void init()
  {
    glutInitWindowSize(global::width, global::height);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(global::app_title);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_NORMALIZE);
  }
}

#endif
