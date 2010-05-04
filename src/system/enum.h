#ifndef _ENUM_H_
#define _ENUM_H_

namespace enumeration {

  enum E_STATE{ GLOBAL_STATE, BUILD_STATE, AMMO_STATE, CARNAGE_STATE };

  // Used by Network classes to determine which WorldObject to create/load
  // ENUM Will be passed from client->server when object creation requested
  enum E_WorldObjType {
      WorldObj_WO, Ammo_WO, Building_WO, DummyAmmo_WO, DummyBuilding_WO
  };

  // Used by Server to decide whether to load into PhysicsEngine or not
  // NONE   : No Physics Simulation Needed for Object
  // DYNAMIC: Physics Simulation object (currently needed in PhysicsEngine)
  // STATIC : Physics Simulation object (not currently needed in PhysicsEngine)
  //          (Not currently  used. Will be an optimization feature for later)
  enum E_PHYSICS{NONE, DYNAMIC, STATIC};

  // Used by Physics class to determine which btRigidBody to use
  enum E_SHAPE {
	  SMALL_SPHERE, MEDIUM_SPHERE, LARGE_SPHERE, SMALL_CUBE
  };

}

#endif
