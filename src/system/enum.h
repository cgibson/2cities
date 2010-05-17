#ifndef _ENUM_H_
#define _ENUM_H_

namespace enumeration {

  typedef enum E_STATE {
    GLOBAL_STATE, BUILD_STATE, AMMO_STATE, CARNAGE_STATE
  } State;

  typedef enum E_SHAPE {
    SMALL_SPHERE, MEDIUM_SPHERE, LARGE_SPHERE, SMALL_CUBE
  } Shape;

  // Used by Server to decide whether to load into PhysicsEngine or not
  // NONE   : No Physics Simulation Needed for Object
  // DYNAMIC: Physics Simulation object (currently needed in PhysicsEngine)
  // STATIC : Physics Simulation object (not currently needed in PhysicsEngine)
  //          (Not currently  used. Will be an optimization feature for later)
  typedef enum E_PHYSICS {
    NONE, DYNAMIC, STATIC
  } PhysicsState;

  // Used by BuildState to determine which face the user has selected
  typedef enum E_FACE {
  		NOTHING, TOP, FACE1, FACE2, FACE3, FACE4, BOTTOM	
  } Face;
	
  // Used by Network classes to determine which WorldObject to create/load
  // ENUM Will be passed from client->server when object creation requested
  typedef enum E_WorldObjType {
      DUMMY_BLOCK, DUMMY_SPHERE, CUSTOM_BLOCK
  } ObjectType;

  typedef enum E_MaterialType {
    YELLOW_MAT, BLUE_MAT, WHITE_MAT
  } MaterialType;
  
}

#endif
