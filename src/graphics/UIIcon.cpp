#include "UIIcon.h"
#include "graphics.h"

UIIcon::UIIcon()
	: UIControl()
{
	_image_tex = 0;
	_alpha_tex = 0;
	_alpha = false;
	_tint = false;
	_tintr = _tintg = _tintb = 1.0;
	_naturalWidth = 0;
	_naturalHeight = 0;
}

UIIcon::~UIIcon()
{
	// nothing yet
}

void UIIcon::init(const char *image, const char *alpha)
{
	UIControl::init(0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0);

	if (alpha != NULL) _alpha = true;

	// load the image
	_image_tex = texldr::loadBMP(image, &_naturalWidth, &_naturalHeight);
	_w = _naturalWidth;
	_h = _naturalHeight;

	// load the alpha map (if specified)
	if (_alpha)
	{
		_alpha_tex = texldr::loadBMP(alpha, NULL, NULL);
	}
}

void UIIcon::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class

	// update our children
	updateChildren(ms);
}

void UIIcon::draw()
{
	// draw the base class
	UIControl::draw();

	// draw this class

	// activate and bind the textures we're using
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _image_tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _alpha_tex);

	// switch to the icon shader
	gfx::uiIconShader.enable();

	// set shader uniforms
	int loc = glGetUniformLocation(gfx::uiIconShader.getProgram(), "image_tex");
	glUniform1i(loc, 0); // tex unit 0
	loc = glGetUniformLocation(gfx::uiIconShader.getProgram(), "alpha_tex");
	glUniform1i(loc, 1); // tex unit 1
	loc = glGetUniformLocation(gfx::uiIconShader.getProgram(), "use_alpha");
	glUniform1i(loc, _alpha);
	loc = glGetUniformLocation(gfx::uiIconShader.getProgram(), "use_tint");
	glUniform1i(loc, _tint);
	loc = glGetUniformLocation(gfx::uiIconShader.getProgram(), "tint_clr");
	glUniform3f(loc, _tintr, _tintg, _tintb);

	// screen space quad
	glPushMatrix();
		glTranslatef(_x + _w / 2.0, _y + _h / 2.0, 0.0);
		glScalef(_w, _h, 1.0);
		glRotatef(_rotAngle, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(-0.5, -0.5);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(0.5, -0.5);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(0.5, 0.5);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(-0.5, 0.5);
		glEnd();
	glPopMatrix();

	// disable shader
	glUseProgram(0);

	// draw our children
	drawChildren();
}
