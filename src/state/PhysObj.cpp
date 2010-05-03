#include "PhysObj.h"
#include "../helper/Vector.h"

btCollisionShape * DummyAmmoUnit::shape = new btSphereShape(
    btScalar(AMMO_RADIUS));
btCollisionShape * DummyBuildingUnit::shape = //new btSphereShape(
//    btScalar(AMMO_RADIUS));
    new btBoxShape(btVector3(
    BLDG_BLOCK_SIDE_LENGTH / 2,
    BLDG_BLOCK_SIDE_LENGTH / 2,
    BLDG_BLOCK_SIDE_LENGTH / 2));
