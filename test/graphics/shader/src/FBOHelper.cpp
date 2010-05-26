/*
 * FBOHelper.cpp
 *
 * Useful helper class that aids in FrameBufferObject (FBO) creation,
 * management, and use.  Allows for multiple target texture buffers
 * (oftenly used for multi-pass renders.)
 *
 * Copyright(c) 2010 - Christopher Gibson
 *
 */

#include "include/FBOHelper.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "include/tex.h"

/*
 * FBO Helper constructor
 */
FBOHelper::FBOHelper( void )
{
  mFboID = 0;
  mDepthID = 0;
  mTextureCount = 0;
  mWidth = 0;
  mHeight = 0;
  enabled = false;
}

/*
 * Initialize the FBO object and generate textures for render targets
 */
bool FBOHelper::initialize( int w, int h, int textureCount )
{
  clear();
  
  mTextureCount = textureCount;
  
  mTextureIDs = (GLuint*)malloc( sizeof(GLuint*) * textureCount);
  
  GLenum bufs[textureCount];
  
  glGenTextures( textureCount, mTextureIDs );
  
  bool r = init_fbo( w, h, true );

  // bind current FBO
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFboID );
  
  // for every texture, generate a texture and add it to the FBO
  for( int i = 0; i < textureCount; i++ )
  {
    // generate new texture
    GenerateTexture( mTextureIDs[i], w, h );
    bufs[i] = GL_COLOR_ATTACHMENT0_EXT + i;
    
    // attach texture to currently bound FBO as color attachment i
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i,
                               GL_TEXTURE_2D, mTextureIDs[i], 0);
  }
                               
  glDrawBuffers( textureCount, bufs);

  // unbind current FBO
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

  // check for errors
  GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
  if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
  {
    clear();
    return false;
  }
  
  return r;
}

/*
 * Internal function to init the FBO object
 */
bool FBOHelper::init_fbo( int w, int h, bool useDepthBuffer )
{  
  mWidth = w;
  mHeight = h;
  
  // create new FBO
  glGenFramebuffersEXT( 1, &mFboID );
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFboID );
  
  glDrawBuffer(GL_NONE);
  
  // attach depth buffer if we need it
  if( useDepthBuffer )
  {
    glGenRenderbuffersEXT( 1, &mDepthID );
    glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mDepthID );
    glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, mWidth, mHeight );
    // attach depth render buffer to FBO
    glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mDepthID );
  }
  
  // check for errors
  GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
  if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
  {
    clear();
    return false;
  }
  // return to normal
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
  return true;
}

/*
 * Enable the current FBO object
 */
bool FBOHelper::enable( void )
{
  if( !enabled && mFboID )
  {
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFboID );
    glPushAttrib( GL_VIEWPORT_BIT );
    glViewport( 0, 0, mWidth, mHeight );
    enabled = true;
  }
  return enabled;
}

/*
 * Disable the current FBO object
 */
bool FBOHelper::disable( void )
{
  if( enabled )
  {
    glPopAttrib();
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    enabled = false;
  }
  
  return !enabled;
}

/*
 * Reset all valuse
 */
void FBOHelper::clear( void )
{
  disable();
  
  // if depth buffer exists
  if( mDepthID )
    glDeleteRenderbuffersEXT( 1, &mDepthID);
  
  // if FBO object exists
  if( mFboID )
    glDeleteRenderbuffersEXT( 1, &mFboID );
  
  mWidth = 0;
  mHeight = 0;
  
  mFboID = 0;
  mDepthID = 0;
  
  mTextureIDs = NULL;
  mTextureCount = 0;
  
  enabled = false;
}
