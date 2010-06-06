#include "ShockwaveEffect.h"
#include "graphics.h"

ShockwaveEffect::ShockwaveEffect()
	: Effect()
{
	_maxSize = 1.0;
	_billboard = false;
}

ShockwaveEffect::~ShockwaveEffect()
{
	// nothing yet
}

void ShockwaveEffect::init(int durationInMS, float maxSize, bool billboard)
{
	Effect::init(durationInMS);
	_maxSize = maxSize;
	_billboard = billboard;
}

void ShockwaveEffect::update(int ms)
{
	Effect::update(ms);
}

void ShockwaveEffect::draw()
{
	// bind and activate the shockwave texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gfx::fxManager.texture(SHOCKWAVE_MOD));

	// switch to the shockwave shader
	gfx::fxManager.shader(SHOCKWAVE)->enable();

	// set uniforms
	int loc = glGetUniformLocation(gfx::fxManager.shader(SHOCKWAVE)->getProgram(), "shockwave_tex");
	glUniform1i(loc, 0); // tex unit 0
	loc = glGetUniformLocation(gfx::fxManager.shader(SHOCKWAVE)->getProgram(), "trail_clr");
	glUniform4f(loc, 0.0, 1.0, 1.0, 1.0);
	loc = glGetUniformLocation(gfx::fxManager.shader(SHOCKWAVE)->getProgram(), "progression");
	glUniform1f(loc, _progression);

	// draw a plane
	glPushMatrix();
		float scale = sqrtf(_progression) * _maxSize;
		if (!_billboard)
		{
			glTranslatef(_position.x(), _position.y(), _position.z());
			glRotatef(90.0, 1.0, 0.0, 0.0);
		}
		else
		{
			// adapted from the following tutorials: (thanks!)
			//     http://www.sleazysoft.com/forums/view.php?id=284
			//     http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat
			//     http://nehe.gamedev.net/data/articles/article.asp?article=19#6

			// compute the look vector
			Vector bbLook = global::camera->position() - _position;
			bbLook.norm();

			// get the billboard's right vector
			Vector bbRight = global::camera->liftVec().cross(bbLook);

			// get the billboard's true up vector
			Vector bbUp = bbLook.cross(bbRight);

			// construct a matrix to billboard the effect
			float mat[16];
			mat[0] = bbRight.x();
			mat[1] = bbRight.y();
			mat[2] = bbRight.z();
			mat[3] = 0.0;
			mat[4] = bbUp.x();
			mat[5] = bbUp.y();
			mat[6] = bbUp.z();
			mat[7] = 0.0;
			mat[8] = bbLook.x();
			mat[9] = bbLook.y();
			mat[10] = bbLook.z();
			mat[11] = 0.0;
			mat[12] = _position.x();
			mat[13] = _position.y();
			mat[14] = _position.z();
			mat[15] = 1.0;

			// multiply this billboarding matrix with the CTM
			glMultMatrixf(mat);
		}
		glScalef(scale, scale, 1.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-0.5, -0.5, 0.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(0.5, -0.5, 0.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(0.5, 0.5, 0.0);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-0.5, 0.5, 0.0);
		glEnd();
	glPopMatrix();

	// disable shader
	glUseProgram(0);
}
