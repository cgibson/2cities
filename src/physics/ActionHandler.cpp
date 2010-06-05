#include "ActionHandler.h"

void ActionHandler::updateAction(btCollisionWorld * world, btScalar timeStep)
{
  int timePassed = (int)(1000 * timeStep);
  //printf("updateAction");
  
  for (unsigned int i = 0; i < physics->physicsBodies.size(); i++)
  {
    physics->physicsBodies[i]->getWorldObject()->think(timePassed);
  }
}

void ActionHandler::debugDraw()
{
}

void ActionHandler::debugDraw(btIDebugDraw * drawPointer)
{
}
