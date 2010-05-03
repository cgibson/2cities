#include "FBOHelper.h"

FBOHelper::FBOHelper( void )
{
  mFboID = 0;
  mDepthID = 0;
  mTexID = 0;
  mWidth = 0;
  mHeight = 0;
  enabled = false;
  mAutoGenerateMipmaps = false;
}


bool FBOHelper::generateShadowFBO( int w, int h )
{
  mWidth = w;
  mHeight = h;

	// generate new FBO
	glGenFramebuffersEXT( 1, &mFboID );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFboID );
	
	// generate depth texture
	glGenTextures( 1, &mDepthID);
	glBindTexture(GL_TEXTURE_2D, mDepthID);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

  // no actual 'picture' will be saved.  only the depth component
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	// allows for sample2Dshadow call in shader
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);//GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA); 

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16_ARB, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// attach the depth texture to the FBO
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, mDepthID, 0);
	
//	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mDepthID);
	
	// check FBO status
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		printf("FBO Error\n");
		return false;
  }
	
	// done, go back to the way we once were
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

bool FBOHelper::initialize( GLuint textureID, int w, int h, bool useTextureBuffer, bool useDepthBuffer, bool autoGenerateMipmaps)
{
  clear();
  
  if( !textureID && useTextureBuffer) return false;
  
  mTexID = textureID;
  mWidth = w;
  mHeight = h;
  mAutoGenerateMipmaps = autoGenerateMipmaps;
  
  // create new FBO
  glGenFramebuffersEXT( 1, &mFboID );
  glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFboID );
  
  // if we are going to use a texture buffer
  if( useTextureBuffer )
  {
    // bind texture
    glBindTexture( GL_TEXTURE_2D, mTexID );
    
    if( mAutoGenerateMipmaps )
    {
      glGenerateMipmapEXT( GL_TEXTURE_2D );
    }
    
    // unbind texture
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    // attach texture to FBO object
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, mTexID, 0 );
  }else{
	  glDrawBuffer(GL_NONE);
	  glReadBuffer(GL_NONE);
  }
  
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

bool FBOHelper::disable( void )
{
  if( enabled )
  {
    glPopAttrib();
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    enabled = false;
    
    if( mTexID && mAutoGenerateMipmaps )
    {
      glBindTexture( GL_TEXTURE_2D, mTexID );
      glGenerateMipmapEXT( GL_TEXTURE_2D );
    }
  }
  
  return !enabled;
}

void FBOHelper::clear( void )
{
  disable();
  if( mDepthID )
    glDeleteRenderbuffersEXT( 1, &mDepthID);
  
  if( mFboID )
    glDeleteRenderbuffersEXT( 1, &mFboID );
  
  mFboID = 0;
  mDepthID = 0;
  mTexID = 0;
  mWidth = 0;
  mHeight = 0;
  mAutoGenerateMipmaps = false;
  enabled = false;
}
