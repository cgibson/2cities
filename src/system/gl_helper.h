#ifndef _GL_HELPER_H_
#define _GL_HELPER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdlib.h>

#include "global.h"

namespace gl{
  void init()
  {
    glutInitWindowSize(global::width, global::height);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(global::app_title);
#ifdef _WIN32
	  GLenum err = glewInit();
	  if (err != GLEW_OK)
	  {
		  fprintf(stderr, "GLEW initialization failed (%d)!\n", err);
		  exit(EXIT_FAILURE);
	  }
#endif
    glEnable(GL_DEPTH_TEST);
    glClearColor(global::fill_color[0], global::fill_color[1], global::fill_color[2], global::fill_color[3]);

    glEnable(GL_NORMALIZE);

    // turn on alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set pixel packing alignment as OGLFT expects
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
}

#endif
