#ifndef _ENUM_H_
#define _ENUM_H_

namespace enumeration {

  typedef enum E_STATE {
    GLOBAL_STATE, BUILD_STATE, AMMO_STATE, CARNAGE_STATE
  } State;

  typedef enum E_SHAPE {
    SHAPE_MIN,
      SHAPE_SPHERE_MIN = SHAPE_MIN,
        SMALL_SPHERE=SHAPE_SPHERE_MIN,
        MEDIUM_SPHERE,
        LARGE_SPHERE,
        FRACTAL_BOMB_SHAPE,
      SHAPE_SPHERE_MAX,
      SHAPE_BOX_MIN=SHAPE_SPHERE_MAX,
        SMALL_CUBE=SHAPE_BOX_MIN,
        WARPED_CUBE_SHAPE,
      SHAPE_BOX_MAX,
    SHAPE_MAX
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
      TYPE_MIN, DUMMY_BLOCK=TYPE_MIN, WARPED_CUBE, DUMMY_SPHERE, FRACTAL_BOMB, CUSTOM_BLOCK, TYPE_MAX
  } ObjectType;

  typedef enum E_MaterialType {
    MAT_TYPE_MIN, YELLOW_MAT=MAT_TYPE_MIN, BLUE_MAT, WHITE_MAT, GREEN_MAT, MAT_TYPE_MAX
  } MaterialType;
  
  typedef enum E_NetworkInterface {
	  N_PRIVATE, N_CLIENT, N_SERVER
  } NetworkInterface;
}

#endif
