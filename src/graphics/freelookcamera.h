#ifndef _FREELOOKCAMERA_H_
#define _FREELOOKCAMERA_H_

#include <math.h>
#include "camera.h"
#include "../system/io.h"

class FreelookCamera : public Camera {
public:
	FreelookCamera();
	~FreelookCamera();

	// initialize the camera
	void init();

	// update the camera every frame
	void update(int ms);

	// input event handlers
	void keyDown(int key, bool special);
	void keyUp(int key, bool special);

private:
	bool _forward;
	bool _backward;
	bool _left;
	bool _right;
	float _theta;
	float _phi;

	static const float MAX_PITCH;
};

#endif

