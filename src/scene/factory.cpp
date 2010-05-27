#include "factory.h"
#include <stdio.h>

///////////////////////////////
#include "../system/io.h"
#include "../network/NetworkManager.h"
#include "../scene/BlackHole.h"
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
    printf("Making block(s)");
    int floorSize = 5;
    int blockNumberBase = 20000;
    int i, j;
    static int blockSetHeight;
    for (i = 0; i < floorSize; i++)
    {
      for (j = 0; j < floorSize; j++)
      {
//        WorldObject anObject = WorldObject(blockNumberBase + 
          BlackHole anObject = BlackHole(blockNumberBase +
          blockSetHeight * floorSize * floorSize + i * floorSize + j,
          0, enumeration::DUMMY_CONE);
        anObject.setPosition(Vector(i - floorSize / 2.0, blockSetHeight + .5, j - floorSize / 2.0));
        networkManager->network->addObject(anObject);
      }
    }
    blockSetHeight++;
  }
//  printf(" The world now contains %d objects.\n", networkManager->network->
}
///////////////////////////////////

void ObjectFactory::setBlueprint(ObjectType ot, float mass, MaterialType mt, Shape sh, Vector size)
{
  Blueprint tmp = Blueprint(ot, mass, mt, sh, size);
  blueprints[ot] = tmp;
}

void ObjectFactory::loadConfig(char* filename)
{
  Blueprint tmp;
  
  // dummy block
  setBlueprint(DUMMY_BLOCK, 1.0f, YELLOW_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  
  // dummy sphere
  setBlueprint(DUMMY_SPHERE, 10.0f, BLUE_MAT, SMALL_SPHERE, Vector(1.0f, 1.0f, 1.0f));
  io::register_key_down(ObjectFactory::makeBlock);
  
  // dummy block
  setBlueprint(CUSTOM_BLOCK, 1.0f, GREEN_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  
  
  setBlueprint(DUMMY_CONE, 1.0f, YELLOW_MAT, SMALL_CONE, Vector(1.0f, 1.0f, 1.0f));
  
  setBlueprint(DUMMY_CYLINDER, 1.0f, YELLOW_MAT, SMALL_CYLINDER, Vector(1.0f, 1.0f, 1.0f));
  setBlueprint(BLACK_HOLE, 10000.0f, YELLOW_MAT, SMALL_SPHERE, Vector(1.0f, 0.0f, 0.f));
  setBlueprint(BLOCK_1_2_1, 2.0f, YELLOW_MAT, BLOCK_1_2_1_SHAPE, Vector(1.0f, 2.0f, 1.0f));
  setBlueprint(BLOCK_2_4_2, 16.0f, YELLOW_MAT, BLOCK_2_4_2_SHAPE, Vector(2.0f, 4.0f, 2.0f));
  setBlueprint(BLOCK_5_1_5, 25.0f, YELLOW_MAT, BLOCK_5_1_5_SHAPE, Vector(5.0f, 1.0f, 5.0f));
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
    else if (sh == SMALL_CONE)
    {
      temp = new btConeShapeZ(bp.getSize().x(), bp.getSize().z());
    }
    else if (sh == SMALL_CYLINDER)
    {
      temp = new btCylinderShape(VtobtV3(bp.getSize()));
    }
    else
    { // dummy shape value for uninitialized values.
      printf("Unrecognized Shape request, object shape");
      printf(" defaulted to something useless.\n");
      temp = new btSphereShape(btScalar(0));
    }
    shapes[type] = temp;
  }
  return shapes[type];
}
