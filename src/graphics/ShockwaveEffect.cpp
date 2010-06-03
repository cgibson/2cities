#include "ShockwaveEffect.h"
#include "graphics.h"

ShockwaveEffect::ShockwaveEffect()
	: Effect()
{
	// nothing yet
}

ShockwaveEffect::~ShockwaveEffect()
{
	// nothing yet
}

void ShockwaveEffect::init(int durationInMS, float maxSize)
{
	Effect::init(durationInMS);
	_maxSize = maxSize;
}

void ShockwaveEffect::update(int ms)
{
	Effect::update(ms);
	if (!_alive) return;
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
		glTranslatef(_position.x(), _position.y(), _position.z());
		glScalef(scale, 1.0, scale);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-0.5, 0.0, 0.5);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(0.5, 0.0, 0.5);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(0.5, 0.0, -0.5);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-0.5, 0.0, -0.5);
		glEnd();
	glPopMatrix();

	// disable shader
	glUseProgram(0);
}
