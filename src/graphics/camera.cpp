#include "camera.h"
#include "../system/global.h"
#include "graphics.h"

const float Camera::LOOK_RADIUS = 10.0;
const float Camera::DEFAULT_SENSITIVITY = 0.1;
const float Camera::DEFAULT_SPEED = 50.0;

Camera::Camera()
{
	_eyePos.x(0.0);
	_eyePos.y(LOOK_RADIUS);
	_eyePos.z(LOOK_RADIUS);
	_lookPos.x(0.0);
	_lookPos.y(LOOK_RADIUS);
	_lookPos.z(0.0);

	// use natural +y axis = up
	_upVec.x(0.0);
	_upVec.y(1.0);
	_upVec.z(0.0);

	// default sensitivity and speed
	_sensitivity = 0.1;
	_speed = 50.0;
}

Camera::~Camera()
{
	// nothing yet
}

void Camera::init()
{
	// hook up event handlers
	io::register_key_down(Camera::keyDownHandler);
	io::register_key_up(Camera::keyUpHandler);
	io::register_mouse_down(Camera::mouseDownHandler);
	io::register_mouse_up(Camera::mouseUpHandler);
	io::register_mouse_wheel(Camera::mouseWheelHandler);

	// hook up console functions
	gfx::hud.console.registerCmd("sensitivity", Camera::changeSensitivity);
	gfx::hud.console.registerCmd("speed", Camera::changeSpeed);
}

void Camera::init(Vector eyePos, Vector lookPos)
{
	_eyePos = eyePos;
	_lookPos = lookPos;
	init();
}

void Camera::project()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)global::width / global::height, 1.0, 500.0);
	gluLookAt(_eyePos.x(), _eyePos.y(), _eyePos.z(),
			  _lookPos.x(), _lookPos.y(), _lookPos.z(),
			  _upVec.x(), _upVec.y(), _upVec.z());
	glMatrixMode(GL_MODELVIEW);
}

Vector Camera::viewVec()
{
	Vector viewVec = _lookPos - _eyePos;
	viewVec.norm();
	return viewVec;
}

Vector Camera::strafeVec()
{
	Vector strafeVec = viewVec().cross(_upVec);
	strafeVec.norm();
	return strafeVec;
}

Vector Camera::liftVec()
{
	Vector liftVec = strafeVec().cross(viewVec());
	liftVec.norm();
	return liftVec;
}

void Camera::keyDown(int key, bool special) {}
void Camera::keyUp(int key, bool special) {}
void Camera::mouseDown(int button) {}
void Camera::mouseUp(int button) {}
void Camera::mouseWheel(int direction) {}

void Camera::changeSensitivity(int argc, char *argv[])
{
	if (argc != 2)
    {
        gfx::hud.console.error("Usage: %s <sensitivity> (default is %f)", argv[0], DEFAULT_SENSITIVITY);
        return;
    }

	global::camera->sensitivity(atof(argv[1]));
    gfx::hud.console.info("Sensitivity set to %f.", global::camera->sensitivity());
}

void Camera::changeSpeed(int argc, char *argv[])
{
	if (argc != 2)
    {
        gfx::hud.console.error("Usage: %s <speed> (default is %f)", argv[0], DEFAULT_SPEED);
        return;
    }

    global::camera->speed(atof(argv[1]));
    gfx::hud.console.info("Speed set to %f.", global::camera->speed());
}

void Camera::keyDownHandler(int key, bool special)
{
	global::camera->keyDown(key, special);
}

void Camera::keyUpHandler(int key, bool special)
{
	global::camera->keyUp(key, special);
}

void Camera::mouseDownHandler(int button)
{
	global::camera->mouseDown(button);
}

void Camera::mouseUpHandler(int button)
{
	global::camera->mouseUp(button);
}

void Camera::mouseWheelHandler(int direction)
{
	global::camera->mouseWheel(direction);
}
