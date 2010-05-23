#ifndef _ORBITALCAMERA_H_
#define _ORBITALCAMERA_H_

#include <math.h>
#include "camera.h"
#include "../system/io.h"
#include "../helper/Vector.h"

class OrbitalCamera : public Camera {
public:
	OrbitalCamera();
	~OrbitalCamera();

	// initialize the camera
	void init();

	// update the camera every frame
	void update(int ms);

	// input event handlers
	void keyDown(int key, bool special);
	void keyUp(int key, bool special);

	// property accessors
	Vector center() const { return _center; }
	void center(Vector center) { _center = center; }

	// create orbital basis vectors to refer to the eye position and center point
	Vector orbitViewVec();
	Vector orbitStrafeVec();
	Vector orbitLiftVec();

private:
	bool _up;
	bool _down;
	bool _left;
	bool _right;
	float _theta;
	float _phi;
	Vector _center;

	static const float MAX_PITCH_DISP;
	static const float MAX_YAW_DISP;
	static const float ORBIT_RADIUS;
	static const float MAX_CAM_PITCH;
	static const float MIN_CAM_PITCH;
	static const float CENTER_BLEED;
};

#endif


