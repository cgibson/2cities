#include "factory.h"
#include "stdio.h"

ObjectFactory::ObjectFactory()
{
  blueprints = (Blueprint*)malloc(sizeof(Blueprint) * MAX_OBJECT_TYPES);
  loadConfig(NULL);
}

void ObjectFactory::loadConfig(char* filename)
{
  Blueprint tmp;
  
  printf("DUMMY SPHERE: %d\n", DUMMY_SPHERE);
  
  // dummy block
  tmp = Blueprint(DUMMY_BLOCK, 1.0f, YELLOW_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  blueprints[DUMMY_BLOCK] = tmp;
  
  // dummy sphere
  tmp = Blueprint(DUMMY_SPHERE, 1.0f, BLUE_MAT, SMALL_SPHERE, Vector(1, 0, 0));
  blueprints[DUMMY_SPHERE] = tmp;
}

Blueprint ObjectFactory::getBlueprint(ObjectType type)
{
  return blueprints[type];
}
