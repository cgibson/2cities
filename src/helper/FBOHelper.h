#ifndef _FBO_HELPER_H_
#define _FBO_HELPER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <GL/glew.h>
#else
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
#include <stdio.h>

class FBOHelper
{
public:
  FBOHelper( void );
  
  bool initialize( GLuint textureID, int w, int h, bool useTextureBuffer = true, bool useDepthBuffer = true, bool autoGenerateMipmaps = false);
  bool generateShadowFBO( int w, int h );
  bool enable( void );
  bool disable( void );
  void clear( void );
  GLuint getDepthID() { return mDepthID; }

protected:
  GLuint mFboID;
  GLuint mDepthID;
  GLuint mTexID;
  int mWidth, mHeight;
  bool mAutoGenerateMipmaps;
  bool enabled;
};

#endif
