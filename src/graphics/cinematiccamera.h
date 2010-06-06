#ifndef _CINEMATICCAMERA_H_
#define _CINEMATICCAMERA_H_

#include <math.h>
#include "camera.h"
#include "../system/io.h"
#include "../helper/Vector.h"

class CinematicCamera : public Camera {
public:
	CinematicCamera();
	~CinematicCamera();

	// initialize the camera
	void init();

	// update the camera every frame
	void update(int ms);

	// property accessors
	Vector center() const { return _center; }
	void center(Vector center) { _center = center; }

	// create orbital basis vectors to refer to the eye position and center point
	Vector orbitViewVec();
	Vector orbitStrafeVec();
	Vector orbitLiftVec();

private:
	float _theta;
	float _phi;
	Vector _center;

	static const float ORBIT_RADIUS;
	static const float MAX_CAM_PITCH;
	static const float MIN_CAM_PITCH;
	static const float AUTO_MOVE_SPEED;
};

#endif


