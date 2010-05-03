#ifndef _ENUM_H_
#define _ENUM_H_

namespace enumeration {

  enum E_STATE{ GLOBAL_STATE, BUILD_STATE, AMMO_STATE, CARNAGE_STATE };

  enum E_SHAPE{SMALL_SPHERE, MEDIUM_SPHERE, LARGE_SPHERE, SMALL_CUBE};

  enum E_PHYSICS{NONE, DYNAMIC, STATIC};

  enum E_WorldObjType {
      WorldObj_WO, Ammo_WO, Building_WO, DummyAmmo_WO, DummyBuilding_WO
  };
  
}

#endif
