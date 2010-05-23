#ifndef _GL_HELPER_H_
#define _GL_HELPER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

namespace gl
{
	extern void init();
}

#endif
