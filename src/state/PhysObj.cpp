#include "PhysObj.h"
#include "Vector.h"

PhysObj::PhysObj() {
   setPosition(Vector(0,0,0));
   setVelocity(Vector(0,0,0));
   setRotation(Vector(0,0,0));
   setRotationMag(0.0f);
   setMass(0.0f);
}

PhysObj::PhysObj(Vector newPos, Vector newVel, Vector newRot, double newRotMag, float newMass) {
   setPosition(newPos);
   setVelocity(newVel);
   setRotation(newRot);
   setRotationMag(newRotMag);
   setMass(newMass);
}

DummyBuildingUnit::DummyBuildingUnit() {
   setPosition(Vector(0,0,0));
   setVelocity(Vector(0,0,0));
   setRotation(Vector(0,0,0));
   setRotationMag(0);
   setMass(0);
   size = 1.0f;
}

DummyBuildingUnit::DummyBuildingUnit(Vector newPos, Vector newRot, double newRotMag, float newMass, float newSize) {
   setPosition(newPos);
   setVelocity(Vector(0,0,0));
   setRotation(newRot);
   setRotationMag(newRotMag);
   setMass(newMass);
   size = newSize;
}
