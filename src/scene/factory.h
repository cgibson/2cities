#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_

#include "blueprint.h"
#include "../system/enum.h"

#define MAX_OBJECT_TYPES 10

using namespace enumeration;

class ObjectFactory{
public:
  ObjectFactory();
  Blueprint getBlueprint(ObjectType);
  void loadConfig(char*);
private:
  Blueprint* blueprints;
};

#endif
