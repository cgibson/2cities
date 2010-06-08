#include "ActionHandler.h"

void ActionHandler::updateAction(btCollisionWorld * world, btScalar timeStep)
{
  int timePassed = (int)(1000 * timeStep);
  WorldObject * wo;
#ifdef DEBUG
//  if (physics->physicsBodies.size() > 0)
//  printf("updateAction %d\n", physics->physicsBodies.size());
#endif
  
  for (unsigned int i = 0; i < physics->physicsBodies.size(); i++)
  {
    wo = physics->physicsBodies[i]->getWorldObject();
    #ifdef DEBUG
//    printf("Object #%d: Position:%s Velocity:%s\n", wo->getID(), wo->getPosition().str(), wo->getVelocity().str());
    #endif
    wo->think(timePassed);
  }
}

void ActionHandler::debugDraw()
{
}

void ActionHandler::debugDraw(btIDebugDraw * drawPointer)
{
}
