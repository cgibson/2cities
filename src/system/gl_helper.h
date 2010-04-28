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
    glClearColor(global::fill_color[0], global::fill_color[1], global::fill_color[2], global::fill_color[3]);

    glEnable(GL_NORMALIZE);
  }
}

#endif
