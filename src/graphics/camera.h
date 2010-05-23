#ifndef _CAMERA_H_
#define _CAMERA_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <stdlib.h>

#include "../system/io.h"
#include "../helper/Vector.h"

class Camera {
public:
	Camera();
	virtual ~Camera();

	// initialize the camera
	virtual void init();
	void init(Vector eyePos, Vector lookPos);

	// update the camera every frame
	virtual void update(int ms) = 0;

	// wipes the projection stack and does the camera projection based on the
	// current eye and look at positions
	void project();

	// input event handlers (do nothing by default)
	virtual void keyDown(int key, bool special);
	virtual void keyUp(int key, bool special);
	virtual void mouseDown(int button);
	virtual void mouseUp(int button);
	virtual void mouseWheel(int direction);

	// camera basis vector accessors (returns NORMALIZED vectors!)
	Vector viewVec();
	Vector strafeVec();
	Vector liftVec();
	Vector position() const { return _eyePos; }

	// speed and sensitivity adjustments
	float sensitivity() const { return _sensitivity; }
	void sensitivity(float sensitivity) { _sensitivity = sensitivity; }
	float speed() const { return _speed; }
	void speed(float speed) { _speed = speed; }

	// don't call these, they're just passthrus to handle catching the input
	// event callbacks
	static void keyDownHandler(int key, bool special);
	static void keyUpHandler(int key, bool special);
	static void mouseDownHandler(int button);
	static void mouseUpHandler(int button);
	static void mouseWheelHandler(int direction);

	// change sensitivity and speed from the console
	static void changeSensitivity(int argc, char *argv[]);
	static void changeSpeed(int argc, char *argv[]);

	static const float LOOK_RADIUS;
	static const float DEFAULT_SPEED;
	static const float DEFAULT_SENSITIVITY;

protected:
	Vector _eyePos;
	Vector _lookPos;
	Vector _upVec;
	float _sensitivity;
	float _speed;
};

#endif
