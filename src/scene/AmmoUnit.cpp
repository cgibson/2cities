#include "AmmoUnit.h"

AmmoUnit::AmmoUnit()
   : WorldObject(0, 0, UNIT_SHAPE) {}

AmmoUnit::AmmoUnit(unsigned int newid, unsigned int newplayerid)
   : WorldObject(newid, newplayerid, UNIT_SHAPE) {}

void AmmoUnit::draw() {
	ammoMat.applyMaterial();
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());
    glColor3f(1,0,0);
    glutSolidSphere(1,10,10);
    glPopMatrix();
}

void AmmoUnit::update(int elapsedTime) {
	setPosition(getPosition() * getVelocity() * (elapsedTime/1000.0f));
}

void AmmoUnit::think(int elapsedTime) {

}
