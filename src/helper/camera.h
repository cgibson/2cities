#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector.h"
#include <math.h>
#include <stdio.h>

class Camera{
protected:
	float _theta;
	float _phi;
	float _distance;

public:
	Vector eye;
	Vector lookAt;
	float fov;
	float near_plane;
	float far_plane;

	int inverted;
	float phiMaxAngle;
	float phiMinAngle;

	Camera() {
		initialize();
	};

	void initialize() {
		initialize(Vector( 0.0f, 0.0f, 0.0f), Vector( 0.0f, 0.0f, 0.0f));
	};

	void initialize(Vector newEye, Vector newLookAt) {
		eye    = newEye;
		lookAt = newLookAt;
		fov = 45.0f;
		near_plane = 1.0f;
		far_plane = 500.0f;

		inverted = -1;
		phiMaxAngle =  M_PI;
		phiMinAngle = -M_PI;

		updateAngles();
	};

	void updateAngles() {
		float dx = lookAt.x() - eye.x();
		float dy = lookAt.y() - eye.y();
		float dz = lookAt.z() - eye.z();

		float dxz = sqrt(pow(dx, 2) + pow(dz, 2));

		_distance = sqrt(pow(dx, 2) +
						 pow(dy, 2) +
						 pow(dz, 2) );
		_theta =  atan2(dx, dz) + M_PI;
		_phi   = -atan2(dy, dxz);

		printf("updateAngles: theta=%4.2f phi=%4.2f distance=%4.2f\n", _theta, _phi, _distance);
		printf("              camera %s lookAt %s\n", eye.str(), lookAt.str());
	};

	void strafe(float fwdbk, float side, float updown) {
		updateAngles();

		Vector delta = Vector(
			-fwdbk*sin(_theta) + side*cos(M_PI-_theta),
											updown,
			-fwdbk*cos(_theta) + side*sin(M_PI-_theta)
		);

		eye    = eye    + delta;
		lookAt = lookAt + delta;
	};

	void strafe(Vector delta) { strafe(delta.x(), delta.y(), delta.z()); };

	Vector orbitNode(Vector node1, Vector node2, float delta_turn, float delta_tilt) {
		float dx = node1.x() - node2.x();
		float dy = node1.y() - node2.y();
		float dz = node1.z() - node2.z();

		float dxz = sqrt(pow(dx, 2) + pow(dz, 2));

		float distance = sqrt(pow(dx, 2) +
						 pow(dy, 2) +
						 pow(dz, 2) );
		float theta =  atan2(dx, dz) + M_PI;
		float phi   = -atan2(dy, dxz);

		theta += delta_turn;

		if(phi + delta_tilt > phiMaxAngle)
			phi = phiMaxAngle;
		else if (phi + delta_tilt < phiMinAngle)
			phi = phiMinAngle;
		else
			phi += delta_tilt;

		Vector ret_value = Vector(
		   sin(theta) * distance * cos(phi),
						distance * sin(phi),
		   cos(theta) * distance * cos(phi)
		) + node1;

		//printf("rotateView: theta=%4.2f phi=%4.2f distance=%4.2f\n", theta, phi, distance);
		//printf("              node1 %s node2 %s\n", node1.str(), node2.str());

		return ret_value;
	};

	void rotateCamera(float delta_turn, float delta_tilt) {
		eye = orbitNode(lookAt, eye, delta_turn, delta_tilt);
	};

	void rotateView(float delta_turn, float delta_tilt) {
		lookAt = orbitNode(eye, lookAt, -delta_turn, delta_tilt * inverted);
	};
};

#endif
