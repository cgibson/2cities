#include "factory.h"
#include "stdio.h"

ObjectFactory::ObjectFactory()
{
#ifdef DEBUG
  printf("ObjectFactory Initializing...");
#endif

  blueprints = (Blueprint*)malloc(sizeof(Blueprint) * MAX_OBJECT_TYPES);
  loadConfig(NULL);

#ifdef DEBUG
  printf("ObjectFactory Initialized!\n");
#endif
}

void ObjectFactory::loadConfig(char* filename)
{
  Blueprint tmp;
  
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
