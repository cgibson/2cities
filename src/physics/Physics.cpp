#include "Physics.h"
#include "../system/global.h"

using namespace std;
//void Physics::emptyWorld()

void Physics::exitPhysics()
{
//  Physics::emptyWorld();
  delete groundBody;
  // remove the collision shapes
  for (int j = 0; j < collisionShapes.size(); j++)
  {
    btCollisionShape * shape = collisionShapes[j];
    collisionShapes[j] = 0;
    delete shape;
  }
  delete world;
  delete solver;
  delete broadphase;
  delete dispatcher;
  delete collConf;
  collisionShapes.clear();
}

void Physics::initPhysics()
{
  collConf = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collConf);
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
           collConf);
  world->setGravity(btVector3(0,-10, 0));
  btCollisionShape * ground = new btBoxShape(btVector3(btScalar(40.),
          btScalar(GROUND_HEIGHT / 2.0), btScalar(40.)));
//  btCollisionShape * ground = new btStaticPlaneShape(btVector3(0,1,0), 50);
  btAlignedObjectArray<btCollisionShape *> collisionShapes;
  collisionShapes.push_back(ground);

  btTransform groundTrans;
  groundTrans.setIdentity();
  groundTrans.setOrigin(btVector3(0,-GROUND_HEIGHT / 2.0,0));
  btRigidBody::btRigidBodyConstructionInfo grbInfo(btScalar(0),
    new btDefaultMotionState(groundTrans), ground, btVector3(0,8,0));
  // Put the ground into the world
  groundBody = new btRigidBody(grbInfo);
  world->addRigidBody(groundBody);
  groundBody->setActivationState(ISLAND_SLEEPING);
  nextBlockNumber = 0;
  world->setInternalTickCallback(&tickCallback);
}

void Physics::update(int timeChange)
{
//  printf("Updating by: %d milliseconds.\n", timeChange);
  vector<WorldObject> changed;
  vector<PhysicsBody *> temp = physicsBodies;
  if (timeChange)
    world->stepSimulation(btScalar(timeChange / 1000.0), 2, btScalar(1 / 25.0));
  unsigned int i;
  int result;
  for (i = 0; i < temp.size(); i++)
  {
		result = temp[i]->update();
    if (result == 1)
    {
      changed.push_back(*(temp[i]->getWorldObject()));
    }
    if (result == 0)
    {
			//removeWorldObject(temp[i]->getWorldObject().getID());
		}
		else
		{
    }
  }
}

int Physics::isUniqueID(int id)
{
  unsigned int i;
  int result = 1;
  for (i = 0; result && i < physicsBodies.size(); i++)
  {
    if (physicsBodies[i]->getID() == id)
    {
      result = 0;
    }
  }
  return result;
}

void Physics::tickCallback(btDynamicsWorld *dWorld, btScalar timeChange)
{
//  printf("The world just ticked by %f seconds!\n", timeChange);
}


void Physics::addWorldObject(WorldObject worldObject)
{
  
  if (isUniqueID(worldObject.getID()))
  {
    PhysicsBody * newBody = new PhysicsBody(worldObject);
    // stall slow objects

    if (worldObject.getVelocity().mag() < .01)
    {
      newBody->setActivationState(ISLAND_SLEEPING);
    }
    world->addRigidBody(newBody->getRigidBody());
    physicsBodies.push_back(newBody);
    printf("sleeping threshholds: %f, %f \n", newBody->getLinearSleepingThreshold(), newBody->getAngularSleepingThreshold());
  }
  else
  {
    // TODO provide a reaction to a non-unique ID value.
  }
}

void Physics::emptyWorld()
{
#ifdef DEBUG
  printf("Function: Physics::emptyWorld() called, removing all PhysicsObjects\n");
#endif
  vector<PhysicsBody *> temp = physicsBodies;
  // blank out the viewed objects
  physicsBodies.clear();
  int i = temp.size();
  // iterate backwards through the existing PhysicsBodies, removing them all.
  while (i-- > 0)
  {
    world->removeRigidBody(temp[i]);
  }
  nextBlockNumber = 0;
}

bool Physics::removeWorldObject(int id)
{
  bool removed = false;
  for (unsigned int i = 0; !removed && i < physicsBodies.size(); i++)
  {
    if (physicsBodies[i]->getID() == id)
    {
      // Get rid of the associated PhysicsBody
      world->removeRigidBody(physicsBodies[i]->getRigidBody());
      physicsBodies.erase(physicsBodies.begin() + i);
      // Update the WorldObject associated with it
      update(0);
      removed = true;
    }
  }
  return removed;
}

vector<WorldObject *> Physics::getWorldObjects()
{
  vector<WorldObject *> result;
  for (unsigned int i = 0; i < physicsBodies.size(); i++)
  {
    result.push_back(physicsBodies[i]->getWorldObject());
  }
  return result;
}

void Physics::InsertNewBlock(ObjectType type, Vector position)
{
  WorldObject * newObj = new WorldObject(nextBlockNumber++, 0, type);
  newObj->setPosition(position);
  addWorldObject(*newObj);
#ifdef DEBUG
  printf("Adding object at %s\n", position.str());
#endif
 }

vector<Vector> Physics::fileToBlockLocations(const char * fileName)
{
  vector<Vector> result;
  FILE * inFile = fopen(fileName, "r");
  if(inFile == NULL) {
	  printf("level not found\n");
	  return result;
  }

  int blockSize, numRead, edge, inner, i,
      x1, y1, z1, x2, y2, z2, xi, yi, zi, xf, yf, zf;
  float bs;
  Vector start, here, off121, off242, off515;
  Vector size121 = global::factory->getBlueprint(BLOCK_1_2_1).getSize();
  Vector size242 = global::factory->getBlueprint(BLOCK_2_4_2).getSize();
  Vector size515 = global::factory->getBlueprint(BLOCK_5_1_5).getSize();
  Vector ox;
  Vector oy;
  Vector oz;
  if ((numRead = fscanf(inFile, "blocksize: %d\n", &blockSize)) == 1)
  {
    bs = blockSize * 1.001;
    ox = Vector(bs - size121.x(), 0, 0);
    oy = Vector(0, size121.y(), 0);
    oz = Vector(0, 0, blockSize - size121.z());
    off121 = size121 * 0.5;
    off242 = Vector(0, size242.y() / 2.0, 0);
    off515 = Vector(size515.x() * 0.5,
                    bs - size515.y() * 0.5,
                    size515.z() * 0.5);
    WorldObject newObj;
    while ((numRead = fscanf(inFile, "%d %d %d %d %d %d\n",&x1, &y1, &z1,
            &x2, &y2, &z2)) == 6)
/*      for (xi = x1; xi < x2; xi += blockSize)
        for (yi = y1; yi < y2; yi += blockSize)
          for (zi = z1; zi < z2; zi += blockSize)
            result.push_back(Vector((float)xi / blockSize * BLDG_BLOCK_SIDE_LENGTH,
                                    (float)yi / blockSize * BLDG_BLOCK_SIDE_LENGTH 
                                      + BLDG_BLOCK_SIDE_LENGTH / 2.0,
                                    (float)zi / blockSize * BLDG_BLOCK_SIDE_LENGTH));*/

    {
      start = Vector(x1, y1, z1);
      xf = (x2 - x1) / blockSize;
      yf = (y2 - y1) / blockSize;
      zf = (z2 - z1) / blockSize;
      for (xi = 0; xi < xf; xi++)
        for (yi = 0; yi < yf; yi++)
          for (zi = 0; zi < zf; zi++)
          {
            edge = 0;
            inner = 1;
            here = start + Vector(xi, yi, zi) * bs;
            if (xi == 0 || zi == 0)
            {
              InsertNewBlock(BLOCK_1_2_1, here + off121);
              InsertNewBlock(BLOCK_1_2_1, here + off121 + oy);
              edge = 1;
              inner = 0;
            }
            if (xi == 0 || zi == zf - 1)
            {
              InsertNewBlock(BLOCK_1_2_1, here + oz + off121);
              InsertNewBlock(BLOCK_1_2_1, here + oz + off121 + oy);
            }
            if (xi == xf - 1|| zi == zf - 1)
            {
              InsertNewBlock(BLOCK_1_2_1, here + ox + oz + off121);
              InsertNewBlock(BLOCK_1_2_1, here + ox + oz + off121 + oy);
              edge = 1;
            }
            if (xi == xf - 1|| zi == 0)
            {
              InsertNewBlock(BLOCK_1_2_1, here + ox + off121);
              InsertNewBlock(BLOCK_1_2_1, here + ox + off121 + oy);
            }
            if (edge == 1)
              for (i = 1; i < blockSize - 1; i++)
              {
                if (xi == 0)
                {
#ifdef DEBUG
              printf("e\n");
#endif
                  InsertNewBlock(BLOCK_1_2_1, here 
                    + Vector(0, 0, i) + off121);
                  InsertNewBlock(BLOCK_1_2_1, here 
                    + Vector(0, 0, i) + oy + off121);
                }
                if (zi == 0)
                {
#ifdef DEBUG
              printf("f\n");
#endif
                  InsertNewBlock(BLOCK_1_2_1, here
                    + Vector(i, 0, 0) + off121);
                  InsertNewBlock(BLOCK_1_2_1, here
                    + Vector(i, 0, 0) + oy + off121);
                }
                if (xi == xf - 1)
                {
#ifdef DEBUG
              printf("g\n");
#endif
                  InsertNewBlock(BLOCK_1_2_1, here 
                    + Vector(0, 0, i) + ox + off121);
                  InsertNewBlock(BLOCK_1_2_1, here 
                    + Vector(0, 0, i) + ox + oy + off121);
                }
                
                if (zi == zf - 1)
                {
#ifdef DEBUG
              printf("h\n");
#endif
                  InsertNewBlock(BLOCK_1_2_1, here
                    + Vector(i, 0, 0) + oz + off121);
                  InsertNewBlock(BLOCK_1_2_1, here
                    + Vector(i, 0, 0) + oz + oy + off121);
                }
            }
            if (inner == 1)
            {
#ifdef DEBUG
                            printf("i\n\n");
#endif

              InsertNewBlock(BLOCK_2_4_2, here + off242);
            }
#ifdef DEBUG
              printf("j\n\n");
#endif
            InsertNewBlock(BLOCK_5_1_5, here + off515);
          }
/*      for (xi = 0; xi <= xf; xi++)
        for (yi = 0; yi < yf; yi++)
          for (zi = 0; zi < zf; zi++)
          {
            if (xi == 0)
            {
          InsertNewBlock(BLOCK_1_2_1, start 
            + (Vector(xi, yi, 0 + i) * blockSize) + Vector(0, 1, 0));
            }
            else if (xi == xf)
            {
            }
          }*/
/*      for (xi = 0; xi < xf; xi++)
        for (yi = 0; yi < yf; yi++)
        {
          for (i = 0; i < 1; i += 1 / blockSize)
          {
          InsertNewBlock(BLOCK_1_2_1, start 
            + (Vector(xi, yi, 0 + i) * blockSize) + Vector(0, 1, 0));
          InsertNewBlock(BLOCK_1_2_1, start 
            + (Vector(xi, yi, 0 + i) * blockSize) + Vector(0, 1, 0));
          InsertNewBlock(BLOCK_1_2_1, start 
            + (Vector(xi, yi, zf + i) * blockSize) + Vector(0, 3, 0));
          InsertNewBlock(BLOCK_1_2_1, start 
            + (Vector(xi, yi, zf + i) * blockSize) + Vector(0, 3, 0));
          }
        }
      for (zi = 1; zi < zf - 1; zi++)
        for (yi = 0; yi < yf; yi++)
        {
          for (i = 0; i < 1; i += 1 / blockSize)
          {
          InsertNewBlock(BLOCK_1_2_1, start
            + (Vector(0 + i, yi, zi) * blockSize) + Vector(0, 1, 0));
          InsertNewBlock(BLOCK_1_2_1, start
            + (Vector(xf + i, yi, zi) * blockSize) + Vector(0, 1, 0));
          InsertNewBlock(BLOCK_1_2_1, start
            + (Vector(0 + i, yi, zi) * blockSize) + Vector(0, 3, 0));
          InsertNewBlock(BLOCK_1_2_1, start
            + (Vector(xf + i, yi, zi) * blockSize) + Vector(0, 3, 0));
          }
        }
      for (xi = 0; xi < xf; xi++)
        for (yi = 0; yi < yf; yi++)
          for (zi = 0; zi < zf; zi++)
          {
            if (xi > 0 && zi > 0)
            {
              InsertNewBlock(BLOCK_2_4_2, start
                + (Vector(xi, yi, zi) * blockSize) + Vector(-1, 2, -1));
            }
            InsertNewBlock(BLOCK_5_1_5, start
              + (Vector(xi, yi, zi)) * blockSize + Vector(-2.5, 4.5, -2.5));
          }
*/            
      
      // Tesselate the input values accordingly
/*      for (xi = x1; xi < x2; xi += blockSize)
        for (yi = y1; yi < y2; yi += blockSize)
        {
          InsertNewBlock(enumeration::BLOCK_1_2_1,
            Vector(
              (float)xi * sz * size121.x(),
              (float)yi * sz * size121.y() + size121.y() * 0.5,
              (float)z1 * sz * size121.z()));
          InsertNewBlock(enumeration::BLOCK_1_2_1,
            Vector(
              (float)xi * sz * size121.x(),
              (float)yi * sz * size121.y() + size121.y() * 0.5,
              (float)z2 * sz * size121.z()));
        }
      for (zi = z1 + blockSize; zi < z2 - blockSize; zi += blockSize)
        for (yi = y1; yi < y2; yi += blockSize)
        {
          InsertNewBlock(enumeration::BLOCK_1_2_1,
            Vector(
              (float)xi * sz * size121.x(),
              (float)yi * sz * size121.y() + size121.y() * 0.5,
              (float)z1 * sz * size121.z()));
          InsertNewBlock(enumeration::BLOCK_1_2_1,
            Vector(
              (float)xi * sz * size121.x(),
              (float)yi * sz * size121.y() + size121.y() * 0.5,
              (float)z2 * sz * size121.z()));
        }
      for (xi = x1 + blockSize; xi < x2; xi += blockSize)
        for (yi = y1; yi < y2; yi += blockSize)
          for (zi = z1 + blockSize; zi < z2; zi += blockSize)
          {
            if (xi < x2 - blockSize && zi < z2 - blockSize)
              InsertNewBlock(enumeration::BLOCK_2_4_2,
                Vector(
                  (float)xi * sz * size242.x(),
                  (float)yi * sz * size242.y()
                    + size242.y() * 0.5,
                  (float)z1 * sz * size242.z()));
            InsertNewBlock(enumeration::BLOCK_5_1_5,
              Vector(
                (float)xi * sz * size242.x() - size242.x() * 0.5,
                (float)yi * sz * size242.y() + size242.y() * 0.5,
                (float)z1 * sz * size242.z() - size242.z() * 0.5));
          }*/
      
    }
    if (numRead != 0)
    {
//      printf("Unexpected data at end of file: ignored\n");
    }
  }
  else
  {
    printf("Unable to read block size.\n");
  }
  return result;
}

// The current implementation only adds blocks to the world, no reinitilization.
int Physics::loadFromFile(const char * fileName)
{
  Physics::emptyWorld();
  /////////////////////////////////////////////////////////////////////////////
  vector<Vector> toPlace = Physics::fileToBlockLocations(fileName);
  // This call will need to change with any movement to the placement of
  // the file load instruction.
  /////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
  printf("Loaded file: \"%s\"\n", fileName);
#endif
/*  WorldObject *newObj;
  for (vector<Vector>::iterator it = toPlace.begin(); it < toPlace.end();
      it++)
  {
    Vector position = *it;

    newObj = new WorldObject(i++, 0, enumeration::DUMMY_BLOCK);
    newObj->setPosition(*it);
    addWorldObject(*newObj);
//    printf("adding object: %d\n", i);

  }*/
  return 1;
}
