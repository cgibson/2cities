#include "BlackholeEffect.h"
#include "graphics.h"

BlackholeEffect::BlackholeEffect()
	: Effect()
{
	// nothing yet
}

BlackholeEffect::~BlackholeEffect()
{
	// nothing yet
}

void BlackholeEffect::init(int durationInMS)
{
	Effect::init(durationInMS);
}

void BlackholeEffect::update(int ms)
{
	Effect::update(ms);
}

void BlackholeEffect::draw()
{
	// bind and activate the shockwave texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gfx::fxManager.texture(SHOCKWAVE_MOD));

	// switch to the shockwave shader
	gfx::fxManager.shader(BLACKHOLE)->enable();

	// set uniforms
	int loc = glGetUniformLocation(gfx::fxManager.shader(BLACKHOLE)->getProgram(), "shockwave_tex");
	glUniform1i(loc, 0); // tex unit 0
	loc = glGetUniformLocation(gfx::fxManager.shader(BLACKHOLE)->getProgram(), "trail_clr");
	glUniform4f(loc, 1.0, 1.0, 0.0, 1.0);
	loc = glGetUniformLocation(gfx::fxManager.shader(BLACKHOLE)->getProgram(), "progression");
	glUniform1f(loc, _progression);

	float scale = (5.0 * (0.5 * sinf(_progression * 20.0 * 2.0 * M_PI) + 0.5)) + 10.0;

	// draw a plane
	glPushMatrix();
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
