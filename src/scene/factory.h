#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_

#include "blueprint.h"
#include "../system/enum.h"
#ifdef SERVER
  #include <vector>
  #include "btBulletDynamicsCommon.h"
#endif

#define MAX_OBJECT_TYPES 10

using namespace enumeration;

class ObjectFactory{
public:
  static void makeBlock(int, bool);
  ObjectFactory();
  Blueprint getBlueprint(ObjectType);
  void loadConfig(char*);
#ifdef SERVER
  btCollisionShape * getShape(ObjectType);
#endif
private:
#ifdef SERVER
  static btVector3 VtobtV3(Vector in)
  {
    return btVector3(btScalar(in.x()), btScalar(in.y()), btScalar(in.z()));
  }

  std::vector<btCollisionShape *> shapes;
#endif
  Blueprint* blueprints;
  bool validBlueprintNumber(ObjectType type)
  {
    return (type >= 0 && type < MAX_OBJECT_TYPES);
  }
};

#endif
