#ifndef _ACTIONHANDLER_H_
#define _ACTIONHANDLER_H_
#include "Physics.h"

class ActionHandler : public btActionInterface
{
  private:
    Physics * physics;
    
  public:
    ActionHandler() {physics = NULL;}
    ActionHandler(Physics * newPhysics) {physics = newPhysics;}
    void setPhysics(Physics * newPhysics) {physics = newPhysics;}
    void updateAction(btCollisionWorld * world, btScalar timeStep);
    void debugDraw();
    void debugDraw(btIDebugDraw * drawPointer);
};

#endif
