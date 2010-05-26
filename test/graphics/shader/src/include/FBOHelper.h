/*
 * FBOHelper.h
 *
 * Useful helper class that aids in FrameBufferObject (FBO) creation,
 * management, and use.  Allows for multiple target texture buffers
 * (oftenly used for multi-pass renders.)
 *
 * Copyright(c) 2010 - Christopher Gibson
 *
 */

#ifndef _FBO_HELPER_H_
#define _FBO_HELPER_H_

#include "GL/gl.h"

class FBOHelper
{
public:
  FBOHelper( void );
  
  bool initialize( int w, int h, int textureCount );
  bool init_fbo( int w, int h, bool useDepthBuffer = true );
  bool generateShadowFBO( int w, int h );
  bool enable( void );
  bool disable( void );
  void clear( void );
  GLuint getDepthID() { return mDepthID; }
  GLuint getFboID() { return mFboID; }
  GLuint* getTextureIDs() { return mTextureIDs; }

protected:
  GLuint mFboID;        // FBO ID
  GLuint mDepthID;      // depth buffer ID
  int mWidth, mHeight;  // width and height of buffers
  bool enabled;         // whether or not the FBO is enabled
  int mTextureCount;    // amount of textures built as render targets
  GLuint* mTextureIDs;  // texture ids used as render targets
};

#endif
