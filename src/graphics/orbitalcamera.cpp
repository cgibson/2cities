#include "orbitalcamera.h"

// handy macros for converting degrees to radians and back
#define DEG2RAD(x) ((x) * M_PI / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

const float OrbitalCamera::MAX_PITCH_DISP = 30.0;
const float OrbitalCamera::MAX_YAW_DISP = 30.0;
const float OrbitalCamera::ORBIT_RADIUS = 75.0;
const float OrbitalCamera::MAX_CAM_PITCH = -5.0;
const float OrbitalCamera::MIN_CAM_PITCH = -55.0; // this + MAX_PITCH_DISP must NOT be greater than 90 degrees!
const float OrbitalCamera::CENTER_BLEED = 0.985;

OrbitalCamera::OrbitalCamera()
	: Camera()
{
	_up = _down = _left = _right = _shift = false;
	_shiftCaptureMode = false;
}

OrbitalCamera::~OrbitalCamera()
{
	// nothing yet
}

void OrbitalCamera::init()
{
	// initialize the base class
	Camera::init();

	// look position is the center of the orbit
	_center = _lookPos;

	// grab view vector and inverse of view vector
	Vector oViewInv = orbitViewVec() * -1;

	// re-adjust the eye position to place it on the orbital sphere
	_eyePos.x(_center.x() + (ORBIT_RADIUS * oViewInv.x()));
	_eyePos.y(_center.y() + (ORBIT_RADIUS * oViewInv.y()));
	_eyePos.z(_center.z() + (ORBIT_RADIUS * oViewInv.z()));

	// initial displacements are zero
	_phi = 0.0;
	_theta = 0.0;
}

void OrbitalCamera::update(int ms)
{
	float lift_amt = 0.0;
	float strafe_amt = 0.0;

	// calculate move amounts based on key presses
	if (_up) lift_amt += _speed * (ms / 1000.0);
	if (_down) lift_amt -= _speed * (ms / 1000.0);
	if (_left) strafe_amt -= _speed * (ms / 1000.0);
	if (_right) strafe_amt += _speed * (ms / 1000.0);

	// grab the orbital lift and strafe vectors
	Vector oLift = orbitLiftVec();
	Vector oStrafe = orbitStrafeVec();

	// calculate the new eye position (as if we were in free look)
	_eyePos.x(_eyePos.x() + (oLift.x() * lift_amt) + (oStrafe.x() * strafe_amt));
	_eyePos.y(_eyePos.y() + (oLift.y() * lift_amt) + (oStrafe.y() * strafe_amt));
	_eyePos.z(_eyePos.z() + (oLift.z() * lift_amt) + (oStrafe.z() * strafe_amt));

	// snap to orbital sphere
	Vector oViewInv = orbitViewVec() * -1;
	_eyePos.x(_center.x() + (ORBIT_RADIUS * oViewInv.x()));
	_eyePos.y(_center.y() + (ORBIT_RADIUS * oViewInv.y()));
	_eyePos.z(_center.z() + (ORBIT_RADIUS * oViewInv.z()));

	// computer camera pitch angle and snap camera against constraints
	float cam_phi = RAD2DEG(asinf(orbitViewVec().y()));
	cam_phi = (cam_phi > MAX_CAM_PITCH) ? MAX_CAM_PITCH : cam_phi;
	cam_phi = (cam_phi < MIN_CAM_PITCH) ? MIN_CAM_PITCH : cam_phi;
	_eyePos.y(_center.y() - (ORBIT_RADIUS * sinf(DEG2RAD(cam_phi)))); // the minus is voodoo. don't touch.

	// if we're moving the camera at all, slowly bleed off the theta/phi displacements
	// back to center (to hopefully keep the user from getting disoriented)
	if (_up || _down || _left || _right)
	{
		_theta *= CENTER_BLEED;
		_phi *= CENTER_BLEED;
	}

	// adjust the theta/phi displacements from the camera values (based on mouse input)
	if (io::captured)
	{
		_theta += io::mouse_x * _sensitivity;
		_phi -= io::mouse_y * _sensitivity;
	}

	// clamp the max displacements
	_phi = (_phi > MAX_PITCH_DISP) ? MAX_PITCH_DISP : _phi;
	_phi = (_phi < -MAX_PITCH_DISP) ? -MAX_PITCH_DISP: _phi;
	_theta = (_theta > MAX_YAW_DISP) ? MAX_YAW_DISP : _theta;
	_theta = (_theta < -MAX_YAW_DISP) ? -MAX_YAW_DISP : _theta;

	// figure out the camera theta and phi values
	Vector oViewVec = orbitViewVec();
	float cam_theta = RAD2DEG(atan2f(oViewVec.z(), oViewVec.x()));
	cam_phi = RAD2DEG(asinf(oViewVec.y()));

	// calculate the absolute theta/phi values by combining camera with displacements
	float abs_theta = cam_theta + _theta;
	float abs_phi = cam_phi + _phi;

	// calculate the look position based on the absolute theta/phi values and eye position
	_lookPos.x(_eyePos.x() + (LOOK_RADIUS * cosf(DEG2RAD(abs_phi)) * cosf(DEG2RAD(abs_theta))));
	_lookPos.y(_eyePos.y() + (LOOK_RADIUS * sinf(DEG2RAD(abs_phi))));
	_lookPos.z(_eyePos.z() + (LOOK_RADIUS * cosf(DEG2RAD(abs_phi)) * sinf(DEG2RAD(abs_theta))));
}

void OrbitalCamera::keyDown(int key, bool special)
{
	if (!special)
	{
		switch (key)
		{
			case 'w':
				_up = true;
				break;

			case 's':
				_down = true;
				break;

			case 'a':
				_left = true;
				break;

			case 'd':
				_right = true;
				break;

			case ' ':
				if (_shiftCaptureMode)
				{
					if (!_shift) io::capture_mouse();
					_shift = true;
				}
				break;

			default:
				break;
		}
	}
}

void OrbitalCamera::keyUp(int key, bool special)
{
	if (!special)
	{
		switch (key)
		{
			case 'w':
				_up = false;
				break;

			case 's':
				_down = false;
				break;

			case 'a':
				_left = false;
				break;

			case 'd':
				_right = false;
				break;

			case ' ':
				if (_shiftCaptureMode)
				{
					if (_shift) io::release_mouse();
					_shift = false;
				}
				break;

			default:
				break;
		}
	}
}

Vector OrbitalCamera::orbitViewVec()
{
	Vector viewVec = _center - _eyePos;
	viewVec.norm();
	return viewVec;
}

Vector OrbitalCamera::orbitStrafeVec()
{
	Vector strafeVec = orbitViewVec().cross(_upVec);
	strafeVec.norm();
	return strafeVec;
}

Vector OrbitalCamera::orbitLiftVec()
{
	Vector liftVec = orbitStrafeVec().cross(orbitViewVec());
	liftVec.norm();
	return liftVec;
}

#undef DEG2RAD
#undef RAD2DEG

