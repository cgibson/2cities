#include "PhysObj.h"
#include "Vector.h"

PhysObj::PhysObj() {
   setPosition(Vector(0,0,0));
   setVelocity(Vector(0,0,0));
   setDirection(Vector(0,0,0));
   setMass(0.0f);
}

PhysObj::PhysObj(Vector newPos, Vector newVel, Vector newDir, float newMass) {
   setPosition(newPos);
   setVelocity(newVel);
   setDirection(newDir);
   setMass(newMass);
}

DummyBuildingUnit::DummyBuildingUnit() {
   size = 1.0f;
}

DummyBuildingUnit::DummyBuildingUnit(Vector newPos, Vector newDir, float newMass, float newSize) {
   setPosition(newPos);
   setVelocity(Vector(0,0,0));
   setDirection(newDir);
   setMass(newMass);
   size = newSize;
}
