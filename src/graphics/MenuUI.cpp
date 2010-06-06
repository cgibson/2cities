#include "MenuUI.h"
#include "graphics.h"

const float MenuUI::SPRING = 10.0;
const float MenuUI::FRICTION = 0.5;
const int MenuUI::INTRO_DURATION_MS = 3000;

MenuUI::MenuUI()
	: GameUI()
{
	_twoLabel = NULL;
	_citiesLabel = NULL;
	_hostGame = NULL;
	_joinGame = NULL;
	_exitGame = NULL;
	_ipLabel = NULL;
	_ipInput = NULL;
	_currentChoice = 0; // host game
	_velocity = 0.0;
	_offset = 0.0;
	_choiceMade = false;
	_intro = true;
	_introStart = -1;
	_introPos = 0;
	_firedShockwave = false;
}

MenuUI::~MenuUI()
{
	if (_twoLabel != NULL) delete _twoLabel;
	if (_citiesLabel != NULL) delete _citiesLabel;
	if (_hostGame != NULL) delete _hostGame;
	if (_joinGame != NULL) delete _joinGame;
	if (_exitGame != NULL) delete _exitGame;
	if (_ipLabel != NULL) delete _ipLabel;
	if (_ipInput != NULL) delete _ipInput;
}

void MenuUI::init()
{
	GameUI::init(0.0, 0.0, 0.0, 0.5);

	_twoLabel = new UILabel();
	_twoLabel->init("resources/fonts/sui_generis_free.ttf", 96, UILabel::RIGHT);
	_twoLabel->fgclr(1.0, 1.0, 1.0, 1.0);
	_twoLabel->text("Two");
	_twoLabel->parent(_window);

	_citiesLabel = new UILabel();
	_citiesLabel->init("resources/fonts/sui_generis_free.ttf", 96, UILabel::LEFT);
	_citiesLabel->fgclr(1.0, 1.0, 1.0, 1.0);
	_citiesLabel->text("Cities");
	_citiesLabel->parent(_window);

	_hostGame = new UILabel();
	_hostGame->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::CENTER);
	_hostGame->pos(global::width / 2, global::height / 2);
	_hostGame->fgclr(1.0, 1.0, 1.0, 1.0);
	_hostGame->text("HOST");
	_hostGame->parent(_window);

	_joinGame = new UILabel();
	_joinGame->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::CENTER);
	_joinGame->pos(global::width / 2, global::height / 2);
	_joinGame->fgclr(1.0, 1.0, 1.0, 1.0);
	_joinGame->text("JOIN");
	_joinGame->parent(_window);

	_exitGame = new UILabel();
	_exitGame->init("resources/fonts/sui_generis_free.ttf", 48, UILabel::CENTER);
	_exitGame->pos(global::width / 2, global::height / 2);
	_exitGame->fgclr(1.0, 1.0, 1.0, 1.0);
	_exitGame->text("EXIT");
	_exitGame->parent(_window);

	_ipLabel = new UILabel();
	_ipLabel->init("resources/fonts/sui_generis_free.ttf", 24, UILabel::RIGHT);
	_ipLabel->fgclr(1.0, 1.0, 1.0, 1.0);
	_ipLabel->text("Server IP:");
	_ipLabel->parent(_window);

	_ipInput = new UILabel();
	_ipInput->init("resources/fonts/sui_generis_free.ttf", 24, UILabel::LEFT);
	_ipInput->fgclr(1.0, 1.0, 1.0, 1.0);
	_ipInput->text("_");
	_ipInput->parent(_window);
}

void MenuUI::update(int ms)
{
	// keep the labels aligned correctly
	_ipLabel->pos(global::width / 2 - 8, global::height / 2 - 250);
	_ipInput->pos(global::width / 2 + 8, global::height / 2 - 250);

	if (_intro)
	{
		// we're doing the intro animation

		// have we set the start time yet?
		if (_introStart < 0) _introStart = global::elapsed_ms();

		// compute our t (0 -> 1) over the duration
		float t = (float)(global::elapsed_ms() - _introStart) / INTRO_DURATION_MS;

		if (t > 1.0) _intro = false; // all done!

		// compute our new bounce location
		_introPos = introBounce(t);

		// position the logo elements
		_twoLabel->pos(global::width / 2 - 75 - _introPos, global::height / 2 + 100);
		_citiesLabel->pos(global::width / 2 - 30 + _introPos, global::height / 2 + 100);

		// time to fire the shockwave?
		if (!_firedShockwave && t >= 1 / 2.75)
		{
			ShockwaveEffect *fx = new ShockwaveEffect();
			fx->init(4000, 45.0, true);
			fx->position(Vector(0.0, 21.0, 0.0));
			gfx::fxManager.addEffect(fx);
			_firedShockwave = true;
		}
	}
	else
	{
		// keep the logo centered
		_twoLabel->pos(global::width / 2 - 75, global::height / 2 + 100);
		_citiesLabel->pos(global::width / 2 - 30, global::height / 2 + 100);
	}

	// animate the start game choice (host/join game)

	// calculate the ideal offset based on the currently selected item
	float idealOffset = _currentChoice * 56;

	// calculate the delta based on a simple spring
	float accel = (idealOffset - _offset) * SPRING;
	_velocity += accel;
	_velocity *= FRICTION;
	float delta = _velocity * (ms / 1000.0);

	// move our current offset by the delta
	_offset += delta;

	// position each choice
	_hostGame->pos(global::width / 2, global::height / 2 - (0 * 56) + _offset - 100);
	_joinGame->pos(global::width / 2, global::height / 2 - (1 * 56) + _offset - 100);
	_exitGame->pos(global::width / 2, global::height / 2 - (2 * 56) + _offset - 100);

	// set the alpha of each choice
	float alpha = 1.0 - (fabsf(global::height / 2.0 - _hostGame->y() - 100) / 100.0); alpha *= alpha; alpha = (alpha < 0.1) ? 0.1 : alpha;
	_hostGame->fgclr(_hostGame->fgr(), _hostGame->fgg(), _hostGame->fgb(), alpha);
	alpha = 1.0 - (fabsf(global::height / 2.0 - _joinGame->y() - 100) / 100.0); alpha *= alpha; alpha = (alpha < 0.1) ? 0.1 : alpha;
	_joinGame->fgclr(_joinGame->fgr(), _joinGame->fgg(), _joinGame->fgb(), alpha);
	alpha = 1.0 - (fabsf(global::height / 2.0 - _exitGame->y() - 100) / 100.0); alpha *= alpha; alpha = (alpha < 0.1) ? 0.1 : alpha;
	_exitGame->fgclr(_exitGame->fgr(), _exitGame->fgg(), _exitGame->fgb(), alpha);

	// compute the alpha of the join ip box
	float idealAlpha = (_choiceMade) ? 1.0 : 0.0;
	accel = (idealAlpha - _ipAlpha) * SPRING;
	_ipAlphaVel += accel;
	_ipAlphaVel *= FRICTION;
	delta = _ipAlphaVel * (ms / 1000.0);
	_ipAlpha += delta;
	_ipLabel->fgclr(_ipLabel->fgr(), _ipLabel->fgg(), _ipLabel->fgb(), _ipAlpha);
	_ipInput->fgclr(_ipInput->fgr(), _ipInput->fgg(), _ipInput->fgb(), _ipAlpha);

	GameUI::update(ms);
}

void MenuUI::draw()
{
	GameUI::draw();
}

void MenuUI::keyDown(int key, bool special)
{
	if (!special)
	{
		switch (key)
		{
			case 27: // escape
				_choiceMade = false;
				break;

			case '\r':
				if (!_choiceMade)
				{
					if (_currentChoice == 0) // host
					{
						startHosting();
					}
					else if (_currentChoice == 1) // join
					{
						_choiceMade = true;
					}
					else // exit
					{
						exit(EXIT_SUCCESS);
					}
				}
				else
				{
					startJoining();
				}
				break;

			case '1': case '2': case '3': case '4': case '5':
			case '6': case '7': case '8': case '9': case '0': case '.':
				if (_choiceMade) addIPchar((char)key);
				break;

			case 8: // backspace
				if (_choiceMade) removeIPchar();
				break;

			default:
				break;
		}
	}
	else
	{
		switch (key)
		{
			case GLUT_KEY_UP:
				if (!_choiceMade) prevChoice();
				break;

			case GLUT_KEY_DOWN:
				if (!_choiceMade) nextChoice();
				break;

			default:
				break;
		}
	}
}

void MenuUI::mouseDown(int button)
{
	if (button == MOUSE_LEFT)
	{
		switch (_currentChoice)
		{
			case 0: // host
				startHosting();
				break;

			case 1: // join
				_choiceMade = true;
				break;

			case 2: // exit
				exit(EXIT_SUCCESS);
				break;

			default:
				break;
		}
	}
	else if (button == MOUSE_RIGHT)
	{
		_choiceMade = false;
	}
}

void MenuUI::mouseWheel(int direction)
{
	if (direction == MOUSE_WHEEL_UP)
	{
		if (!_choiceMade) prevChoice();
	}
	else
	{
		if (!_choiceMade) nextChoice();
	}
}

void MenuUI::prevChoice()
{
	_currentChoice--;
	_currentChoice = (_currentChoice < 0) ? 0 : _currentChoice;
}

void MenuUI::nextChoice()
{
	_currentChoice++;
	_currentChoice = (_currentChoice > 2) ? 2 : _currentChoice;
}

void MenuUI::startHosting()
{
	// change the network interface to network server
	global::networkManager->changeNetworkInterface(N_SERVER);
}

void MenuUI::startJoining()
{
	// change the network interface to network client
	global::networkManager->changeNetworkInterface(N_CLIENT);

	// get the ip string
	int len = strlen(_ipInput->text());
	char *ip = (char *)malloc(sizeof(char) * len);
	memcpy(ip, _ipInput->text(), len - 1);
	ip[len - 1] = '\0';

	// connect! (hopefully...)
	global::networkManager->network->serverConnect(ip, 5060);

	free(ip);
}

void MenuUI::addIPchar(char c)
{
	int len = strlen(_ipInput->text());
	char *ip = (char *)malloc(sizeof(char) * (len + 2));
	memcpy(ip, _ipInput->text(), len - 1);
	ip[len - 1] = c;
	ip[len] = '_';
	ip[len + 1] = '\0';
	_ipInput->text(ip);
	free(ip);
}

void MenuUI::removeIPchar()
{
	int len = strlen(_ipInput->text());
	if (len == 1) return; // can't remove any more
	char *ip = (char *)malloc(sizeof(char) * len);
	memcpy(ip, _ipInput->text(), len - 2);
	ip[len - 2] = '_';
	ip[len - 1] = '\0';
	_ipInput->text(ip);
	free(ip);
}

float MenuUI::introBounce(float t)
{
	float d = global::width / 2 - 332 - 75;

	if (t < 1 / 2.75)
	{
		return -d * (7.5625 * t * t) + d;
	}
	else if (t < 2 / 2.75)
	{
		t = t - 1.5 / 2.75;
        return -d * (7.5625 * t * t + 0.75) + d;
	}
	else if (t < 2.5 / 2.75)
	{
		t = t - 2.25 / 2.75;
		return -d * (7.5625 * t * t + 0.9375) + d;
	}

	t = t - 2.625 / 2.75;
	return -d * (7.5625 * t * t + 0.984375) + d;
}
