#include "UIScoreDisplay.h"

UIScoreDisplay::UIScoreDisplay()
	: UIControl()
{
	_explainLabel = NULL;
	_scoreLabel = NULL;
	_score = 0;
}

UIScoreDisplay::~UIScoreDisplay()
{
	if (_explainLabel != NULL) delete _explainLabel;
	if (_scoreLabel != NULL) delete _scoreLabel;
}

void UIScoreDisplay::init(ScoreDisplay player)
{
	UIControl::init(100, 100, 100, 100, 0.0, 0.0, 0.0, 1.0);

	_player = player;

	_explainLabel = new UILabel();
	if (_player == LEFT_PLAYER)
	{
		_explainLabel->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::LEFT);
		_explainLabel->pos(5, 55);
		_explainLabel->text("RED SCORE");
	}
	else
	{
		_explainLabel->init("resources/fonts/sui_generis_free.ttf", 12, UILabel::RIGHT);
		_explainLabel->pos(330 - 5, 55);
		_explainLabel->text("BLUE SCORE");
	}
	_explainLabel->parent(this);

	_scoreLabel = new UILabel();
	if (_player == LEFT_PLAYER)
	{
		_scoreLabel->init("resources/fonts/sui_generis_free.ttf", 36, UILabel::LEFT);
		_scoreLabel->pos(8, 8);
	}
	else
	{
		_scoreLabel->init("resources/fonts/sui_generis_free.ttf", 36, UILabel::RIGHT);
		_scoreLabel->pos(330 - 8, 8);
	}
	printScoreToBuffer();
	_scoreLabel->text(_scoreBuf);
	_scoreLabel->shadow(true);
	_scoreLabel->parent(this);
}

void UIScoreDisplay::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class
	_y = global::height - _h;
	if (_player == RIGHT_PLAYER)
	{
		_x = global::width - _w;
	}

	// TODO: eventually fetch the actual score from the game state here
	_score += ms;
	_score %= 1000000;
	printScoreToBuffer();
	_scoreLabel->text(_scoreBuf);

	// update our children
	updateChildren(ms);
}

void UIScoreDisplay::draw()
{
	// DON'T draw the base class
	//UIControl::draw();

	// draw this class
	glPushMatrix();
		glBegin(GL_QUADS);
			glColor4f(_bgr, _bgg, _bgb, _bga);
			switch (_player)
			{
				case LEFT_PLAYER:
					glVertex2f(_x, _y);
					glVertex2f(_x + _w - 50, _y);
					glVertex2f(_x + _w - 50, _y + _h);
					glVertex2f(_x, _y + _h);
					break;

				case RIGHT_PLAYER:
					glVertex2f(_x + 50, _y);
					glVertex2f(_x + _w, _y);
					glVertex2f(_x + _w, _y + _h);
					glVertex2f(_x + 50, _y + _h);
					break;

				default:
					break;
			}
		glEnd();
		glBegin(GL_TRIANGLES);
			glColor4f(_bgr, _bgg, _bgb, _bga);
			switch (_player)
			{
				case LEFT_PLAYER:
					glVertex2f(_x + _w - 50, _y);
					glVertex2f(_x + _w, _y + _h);
					glVertex2f(_x + _w - 50, _y + _h);
					break;

				case RIGHT_PLAYER:
					glVertex2f(_x + 50, _y);
					glVertex2f(_x + 50, _y + _h);
					glVertex2f(_x, _y + _h);
					break;

				default:
					break;
			}
		glEnd();
	glPopMatrix();

	// draw our children
	drawChildren();
}

void UIScoreDisplay::printScoreToBuffer()
{
	snprintf(_scoreBuf, 10, "%d", _score);
}
