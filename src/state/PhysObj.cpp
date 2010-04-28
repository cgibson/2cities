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

AmmoUnit::AmmoUnit() : PhysObj() {
   size = 1.0f;
}

BuildingUnit::BuildingUnit() : PhysObj() {
   size = 1.0f;
}

BuildingUnit::BuildingUnit(Vector newPos, Vector newRot, double newRotMag, float newMass, float newSize)
 : PhysObj(newPos, Vector(0,0,0), newRot, newRotMag, newMass) {

   size = newSize;
}
