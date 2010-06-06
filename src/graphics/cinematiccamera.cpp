#include "cinematiccamera.h"

// handy macros for converting degrees to radians and back
#define DEG2RAD(x) ((x) * M_PI / 180.0)
#define RAD2DEG(x) ((x) * 180.0 / M_PI)

const float CinematicCamera::ORBIT_RADIUS = 75.0;
const float CinematicCamera::MAX_CAM_PITCH = -5.0;
const float CinematicCamera::MIN_CAM_PITCH = -55.0; // this + MAX_PITCH_DISP must NOT be greater than 90 degrees!
const float CinematicCamera::AUTO_MOVE_SPEED = 7.0;

CinematicCamera::CinematicCamera()
	: Camera()
{
	// nothing yet
}

CinematicCamera::~CinematicCamera()
{
	// nothing yet
}

void CinematicCamera::init()
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

void CinematicCamera::update(int ms)
{
	float lift_amt = 0.0;
	float strafe_amt = 0.0;

	// animate the strafe amount
	strafe_amt += AUTO_MOVE_SPEED * (ms / 1000.0);

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

	_lookPos.x(_center.x());
	_lookPos.y(_center.y());
	_lookPos.z(_center.z());
}

Vector CinematicCamera::orbitViewVec()
{
	Vector viewVec = _center - _eyePos;
	viewVec.norm();
	return viewVec;
}

Vector CinematicCamera::orbitStrafeVec()
{
	Vector strafeVec = orbitViewVec().cross(_upVec);
	strafeVec.norm();
	return strafeVec;
}

Vector CinematicCamera::orbitLiftVec()
{
	Vector liftVec = orbitStrafeVec().cross(orbitViewVec());
	liftVec.norm();
	return liftVec;
}

#undef DEG2RAD
#undef RAD2DEG

