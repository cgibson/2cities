#include "WorldObject.h"

void WorldObject::draw() {
	Vector pos = getPosition();
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());
    glColor3f(1,0,0);
    glutSolidSphere(1,10,10);
    glPopMatrix();
}

void WorldObject::update(int elapsedTime) {
	Vector pos = getPosition();
	Vector vel = getVelocity();
	Vector acc = getForce();
	vel = vel + (acc * (elapsedTime/1000.0f));
	pos = pos + (vel * (elapsedTime/1000.0f));

	if(pos.y() > 0) {
		setPosition(pos);
		setVelocity(vel);
	}
	else {
		setPosition(Vector(pos.x(),0,pos.z()));
		setVelocity(Vector(0,0,0));
	}
}

void WorldObject::think(int elapsedTime) {

}
