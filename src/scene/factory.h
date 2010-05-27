#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_

#include "blueprint.h"
#include "../system/enum.h"
#include <vector>
#include "btBulletDynamicsCommon.h"

#define MAX_OBJECT_TYPES 100

using namespace enumeration;

class ObjectFactory{
public:
  static void makeBlock(int, bool);
  ObjectFactory();
  Blueprint getBlueprint(ObjectType);
  void loadConfig(char*);
  btCollisionShape * getShape(ObjectType);
private:
  static btVector3 VtobtV3(Vector in)
  {
    return btVector3(btScalar(in.x()), btScalar(in.y()), btScalar(in.z()));
  }
  void setBlueprint(ObjectType, float, MaterialType, Shape, Vector);

  std::vector<btCollisionShape *> shapes;
  Blueprint* blueprints;
  bool validBlueprintNumber(ObjectType type)
  {
    return (type >= 0 && type < MAX_OBJECT_TYPES);
  }
};

#endif
