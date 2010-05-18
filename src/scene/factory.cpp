#include "factory.h"
#include <stdio.h>

///////////////////////////////
#include "../system/io.h"
#include "../network/NetworkManager.h"
///////////////////////////////

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

///////////////////////////////////
void ObjectFactory::makeBlock(int key, bool special)
{
  if ((key == 'b' || key == 'B') && !special)
  {
    int floorSize = 5;
    int blockNumberBase = 20000;
    static int blockNumber;
    int i, j;
    for (i = 0; i < floorSize; i++)
    {
      for (j = 0; j < floorSize; j++)
      {
        WorldObject anObject = WorldObject(blockNumberBase + (blockNumber + i * floorSize + j), 0, enumeration::DUMMY_BLOCK);
        anObject.setPosition(Vector(i - floorSize / 2.0,blockNumber + .5, j - floorSize / 2.0));
        networkManager->network->addObject(anObject);
      }
    }
    blockNumber++;
  }
}
///////////////////////////////////

void ObjectFactory::loadConfig(char* filename)
{
  Blueprint tmp;
  
  // dummy block
  tmp = Blueprint(DUMMY_BLOCK, 1.0f, YELLOW_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  blueprints[DUMMY_BLOCK] = tmp;
  
  // dummy sphere
  tmp = Blueprint(DUMMY_SPHERE, 1.0f, BLUE_MAT, SMALL_SPHERE, Vector(1, 0, 0));
  blueprints[DUMMY_SPHERE] = tmp;
  io::register_key_down(ObjectFactory::makeBlock);
  
  // dummy block
  tmp = Blueprint(CUSTOM_BLOCK, 1.0f, GREEN_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  blueprints[CUSTOM_BLOCK] = tmp;
}


Blueprint ObjectFactory::getBlueprint(ObjectType type)
{
#ifdef DEBUG
//  printf("Blueprint requested: %d\n", type);
#endif
  if (type < MAX_OBJECT_TYPES && type >= 0)
    return blueprints[type];
  printf("factory.getBlueprint(%d): Invalid ObjectType number. Returning blueprint 0\n", type);
  return blueprints[0];
}

btCollisionShape * ObjectFactory::getShape(ObjectType type)
{
  if (!validBlueprintNumber(type))
  {
    printf("factory.getShape(%d): Invalid ObjectType number\n", type);
    return getShape(DUMMY_BLOCK);
  }
  while (shapes.size() <= (unsigned int)type)
    shapes.push_back(NULL);
  btCollisionShape * temp =  NULL;
  if (shapes[type] == NULL)
  {
    Blueprint bp = getBlueprint(type);
    Shape sh = bp.getShape();
    if (sh >= SHAPE_SPHERE_MIN && sh < SHAPE_SPHERE_MAX)
    {
      temp = new btSphereShape(btScalar(bp.getSize().mag()));
    }
    else if (sh >= SHAPE_BOX_MIN && sh < SHAPE_BOX_MAX)
    {
      btVector3 tv = VtobtV3(bp.getSize()) * .5;
      temp = new btBoxShape(tv);
    }
    else
    { // dummy shape value for uninitialized values.
      temp = new btSphereShape(btScalar(0));
    }
    shapes[type] = temp;
  }
  return shapes[type];
}
