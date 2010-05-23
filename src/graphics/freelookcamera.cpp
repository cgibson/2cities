#include "freelookcamera.h"

// handy macros for converting degrees to radians and back
#define DEG2RAD(x) ((x) * M_PI / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

const float FreelookCamera::MAX_PITCH = 75.0;

FreelookCamera::FreelookCamera()
	: Camera()
{
	_forward = _backward = _left = _right = false;
}

FreelookCamera::~FreelookCamera()
{
	// nothing yet
}

void FreelookCamera::init()
{
	// initialize the base class
	Camera::init();

	// grab the view vector
	Vector view = viewVec();

	// calculate theta and phi
	_phi = RAD2DEG(asinf(view.y()));
	_theta = RAD2DEG(atan2f(view.z(), view.x()));
}

void FreelookCamera::update(int ms)
{
	// don't update if not captured
	if (!io::captured) return;

	float view_amt = 0.0;
	float strafe_amt = 0.0;

	if (_forward) view_amt += _speed * (ms / 1000.0);
	if (_backward) view_amt -= _speed * (ms / 1000.0);
	if (_left) strafe_amt -= _speed * (ms / 1000.0);
	if (_right) strafe_amt += _speed * (ms / 1000.0);

	Vector view = viewVec();
	Vector strafe = strafeVec();

	_eyePos.x(_eyePos.x() + (view.x() * view_amt) + (strafe.x() * strafe_amt));
	_eyePos.y(_eyePos.y() + (view.y() * view_amt) + (strafe.y() * strafe_amt));
	_eyePos.z(_eyePos.z() + (view.z() * view_amt) + (strafe.z() * strafe_amt));

	_theta += io::mouse_x * _sensitivity;
	_phi -= io::mouse_y * _sensitivity;

	_phi = (_phi > MAX_PITCH) ? MAX_PITCH : _phi;
	_phi = (_phi < -MAX_PITCH) ? -MAX_PITCH: _phi;

	_lookPos.x(_eyePos.x() + (LOOK_RADIUS * cosf(DEG2RAD(_phi)) * cosf(DEG2RAD(_theta))));
	_lookPos.y(_eyePos.y() + (LOOK_RADIUS * sinf(DEG2RAD(_phi))));
	_lookPos.z(_eyePos.z() + (LOOK_RADIUS * cosf(DEG2RAD(_phi)) * sinf(DEG2RAD(_theta))));
}

void FreelookCamera::keyDown(int key, bool special)
{
	if (!special)
	{
		switch (key)
		{
			case 'w':
				_forward = true;
				break;

			case 's':
				_backward = true;
				break;

			case 'a':
				_left = true;
				break;

			case 'd':
				_right = true;
				break;

			default:
				break;
		}
	}
}

void FreelookCamera::keyUp(int key, bool special)
{
	if (!special)
	{
		switch (key)
		{
			case 'w':
				_forward = false;
				break;

			case 's':
				_backward = false;
				break;

			case 'a':
				_left = false;
				break;

			case 'd':
				_right = false;
				break;

			default:
				break;
		}
	}
}

#undef DEG2RAD
#undef RAD2DEG
