#ifndef _TEXLOADER_H_
#define _TEXLOADER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>

namespace texldr
{
    // takes a bmp file, returns an OpenGL texture id
    GLuint loadBMP(const char *filename);

    // don't call these, internal use only
    //static unsigned int getuint(FILE *fp);
    //static unsigned short getushort(FILE *fp);
}

#endif
