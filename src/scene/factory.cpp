#include "factory.h"
#include <stdio.h>

///////////////////////////////
#include "../system/io.h"
#include "../network/NetworkManager.h"
#include "../scene/BlackHole.h"
#include "../scene/Shotgun.h"
#include "../scene/AirStrike.h"
#include "../scene/ClusterBomb.h"
#include "../scene/ShapeShifter.h"
#include "../scene/ComplexObject.h"
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
      WorldObject *anObject = new WorldObject(blockNumberBase +
//        BlackHole *anObject = new BlackHole(blockNumberBase +
                                         blockSetHeight * floorSize * floorSize +
                                         i * floorSize + j,
                                       0, enumeration::BALLHEMOTH);
        anObject->setPosition(Vector(i - floorSize / 2.0, blockSetHeight + .5, j - floorSize / 2.0));
        networkManager->network->addObject(anObject);
      }
    }
    blockSetHeight++;
  }
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
  setBlueprint(DUMMY_SPHERE, 10.0f, BLUE_MAT, SMALL_SPHERE, Vector(0.3f, 0.3f, 0.3f));
  io::register_key_down(ObjectFactory::makeBlock);
  
  // dummy block
  setBlueprint(CUSTOM_BLOCK, 1.0f, GREEN_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  
  // dummy cone
  setBlueprint(DUMMY_CONE, 1.0f, YELLOW_MAT, SMALL_CONE, Vector(1.0f, 1.0f, 1.0f));
  
  // recognizer
  setBlueprint(RECOGNIZER, 1000.0f, YELLOW_MAT, RECOGNIZER_SHAPE, Vector(8.0f, 8.0f, 8.0f));
  
  // smooth cannon
  setBlueprint(SMOOTH, 10.0f, YELLOW_MAT, SMOOTH_CANNON_SHAPE, Vector(5.0f, 5.0f, 5.0f));
  
  // twist cannon
  setBlueprint(TWIST, 10.0f, YELLOW_MAT, TWIST_CANNON_SHAPE, Vector(1.0f, 1.0f, 1.0f));
  
  // buster cannon
  setBlueprint(BUSTER, 10.0f, YELLOW_MAT, BUSTER_CANNON_SHAPE, Vector(1.0f, 1.0f, 1.0f));
  
  // fractal bomb
  setBlueprint(FRACTAL_BOMB, 10.0f, YELLOW_MAT, FRACTAL_BOMB_SHAPE, Vector(0.5f, 0.5f, 0.5f));
  
  // warped cube
  setBlueprint(WARPED_CUBE, 10.0f, YELLOW_MAT, WARPED_CUBE_SHAPE, Vector(1.0f, 1.0f, 1.0f));
  
  // drill cone
  setBlueprint(DRILL_CONE, 10.0f, YELLOW_MAT, DRILL_CONE_SHAPE, Vector(1.0f, 1.0f, 1.0f));
  
  setBlueprint(DUMMY_CYLINDER, 1.0f, YELLOW_MAT, SMALL_CYLINDER, Vector(1.0f, 1.0f, 1.0f));
  setBlueprint(BLOCK_1_2_1, 2.0f, GREEN_MAT, BLOCK_1_2_1_SHAPE, Vector(1.0f, 2.0f, 1.0f));
  setBlueprint(BLOCK_2_4_2, 16.0f, YELLOW_MAT, BLOCK_2_4_2_SHAPE, Vector(1.0f, 4.0f, 1.0f));
  setBlueprint(BLOCK_5_1_5, 25.0f, YELLOW_MAT, BLOCK_5_1_5_SHAPE, Vector(5.0f, 1.0f, 5.0f));
  setBlueprint(BALLHEMOTH, 100.0f, GREEN_MAT, LARGE_SPHERE, Vector(2.2f, 2.2f, 2.2f));
  setBlueprint(SHOTGUN, 50.0f, YELLOW_MAT, SMALL_SPHERE, Vector(0.3f, 0.3f, 0.3f));
  setBlueprint(AIR_STRIKE, 10.0f, BLUE_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  setBlueprint(CLUSTER_BOMB, 100.0f, BLUE_MAT, SMALL_SPHERE, Vector(0.3f, 0.3f, 0.3f));
  setBlueprint(SHAPE_SHIFTER, 1.0f, BLUE_MAT, SMALL_CUBE, Vector(1.0f, 1.0f, 1.0f));
  setBlueprint(BLACK_HOLE, 1000.0f, YELLOW_MAT, SMALL_SPHERE, Vector(0.5f, 0.0f, 0.f));
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

/* Creates a WorldObject* from derived classes (as required for polymorphism.)
 *
 * If simply a different shape, then no addition is needed as other factory
 * features will take care of this. As such, only add another "case" if the new
 * items is actually a derived class with different update, think, and/or
 * extra data items.
 */
WorldObject* ObjectFactory::makeObject(ObjectType type) {
	WorldObject *retObjPtr;

	switch (type)
	{
	case RECOGNIZER :
		retObjPtr = new ComplexObject(TRON_RECOGNIZER);
		break;
	case SMOOTH :
		retObjPtr = new ComplexObject(SMOOTH_CANNON);
		break;
	case TWIST :
		retObjPtr = new ComplexObject(TWIST_CANNON);
		break;
	case BUSTER :
		retObjPtr = new ComplexObject(BUSTER_CANNON);
		break;
	case FRACTAL_BOMB :
		retObjPtr = new ComplexObject(FRACTAL_BOMB_AMMO);
		break;
	case WARPED_CUBE :
		retObjPtr = new ComplexObject(WARPED_CUBE_AMMO);
		break;
	case DRILL_CONE :
		retObjPtr = new ComplexObject(DRILL_CONE_AMMO);
		break;
	case BLACK_HOLE :
		retObjPtr = new BlackHole();
		break;
  case SHOTGUN :
    retObjPtr = new Shotgun();
    break;
  case AIR_STRIKE :
    retObjPtr = new AirStrike();
    break;
  case SHAPE_SHIFTER :
    retObjPtr = new ShapeShifter();
    break;
  case CLUSTER_BOMB :
    retObjPtr = new ClusterBomb();
    break;

	// Explicitly listing the dummy objects although not needed due to default
	case DUMMY_SPHERE :
	case DUMMY_CYLINDER :
	case DUMMY_CONE :
	default:
		retObjPtr = new WorldObject();
	}

	retObjPtr->setType(type);
	return retObjPtr;
}
