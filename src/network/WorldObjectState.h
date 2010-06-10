#ifndef _WORLDOBJECTSTATE_H_
#define _WORLDOBJECTSTATE_H_

#include <math.h>
#include <vector>
#include <algorithm>

#include "Network.h"
#include "../system/global.h"

class WorldObjectState {
public:
	WorldObject *objPtr;

	unsigned int objID;
	unsigned int playerID;
	unsigned int bldgID;

	float  velMag;

	// Initial Details
	Vector initPos;
	float  initPosDelta;

	// Delta Details
	int    lastUpdate;
	Vector lastPos;
	float  lastPosDelta;

	// Priority (to be reduced each send down to zero)
	int    priority;
	int    lastSent;

	// Lifecycle items
	bool   damaged;

	WorldObjectState(WorldObject * newObj, int initPriority = 2) {
//		objPtr       = newObj;
		objPtr       = global::factory->makeObject(newObj->getType());
		objPtr->import(newObj);

		objID        = newObj->getID();
		playerID     = newObj->getPlayerID();
		bldgID       = newObj->getBldgID();

		velMag       = newObj->getForce().x();

		initPos      = newObj->getPosition();
		initPosDelta = 0;

		lastPos      = initPos;
		lastPosDelta = 0;

		lastUpdate   = newObj->getTimeStamp();
		lastSent     = newObj->getTimeStamp();
		priority     = initPriority;

		damaged      = false;
	}

	~WorldObjectState() {
		// DO NOT DELETE WorldObject Pointer... Physics will do that!
	}

	void update(WorldObject * newObj) {
		// Don't need to import data as its already itself
		objPtr->import(newObj);

		lastUpdate   = objPtr->getTimeStamp();
			
		update();
	}

	void update() {
		if(objPtr != NULL) {
			velMag       = objPtr->getForce().x();
			initPosDelta = (objPtr->getPosition() - initPos).mag();
			lastPosDelta = (objPtr->getPosition() - lastPos).mag();
			lastPos      = objPtr->getPosition();

			if(damaged)
				objPtr->setPhysics(STATIC);

			if(!damaged && initPosDelta > net::OBJECT_KILL_DELTA) {
				damaged = true;
				objPtr->setPhysics(STATIC);
				priority += 1;
			}
		}
	}

	void kill() {
		objPtr = NULL;
		damaged = true;
		priority = -1;
	}

	void sent(int currTime) {
		if(priority > 0)
			--priority;
		lastSent = currTime;
	}

	static void updateVector(std::vector<WorldObjectState> *objVec, WorldObject *objPtr, int newPriority = 2) {
		unsigned int i = 0;

		// *** Linear Search Code ***
		while (i < objVec->size() && (*objVec)[i].objID != objPtr->getID()) { i++; }

		// *** Add, Insert, Update Code ***
		// ObjID Not Found (and at end)... Push_back
		if (i == objVec->size()) {
			objVec->push_back(WorldObjectState(objPtr, newPriority));
			//printf("Added Object\n");
		}
		// ObjID Found, Replace Data
		else {
			(*objVec)[i].update(objPtr);
			(*objVec)[i].priority = newPriority;
			//printf("Updated Object\n");
		}
	}

	static void removeVector(std::vector<WorldObjectState> *objVec, unsigned int objID) {
		unsigned int i=0;

		// *** Linear Search Code ***
		while (i < objVec->size() && (*objVec)[i].objID != objID) { i++; }

		// ObjID found, erase entry
		if (i < objVec->size()) {
			(*objVec)[i].kill(); // ObjectState Remains, just releases object.
			//objVec->erase(objVec->begin()+i);
			//printf("Killed Object\n");
		}
	}

	static void sortVector(std::vector<WorldObjectState> *objVec) {
		std::sort((*objVec).begin(),(*objVec).end(), WorldObjectState::NetworkPrioritySort);
	}

	static bool NetworkPrioritySort(WorldObjectState objSt1, WorldObjectState objSt2) {
		if(objSt1.priority != objSt2.priority)
			return (objSt1.priority > objSt2.priority);

		if(abs(objSt1.velMag - objSt2.velMag) > 0.5f) //(5*net::timeElapsed/1000.0f))
			return (objSt1.velMag > objSt2.velMag);

		if(abs(objSt1.lastPosDelta - objSt2.lastPosDelta) > 0.5f) //(5*net::timeElapsed/1000.0f))
			return (objSt1.lastPosDelta > objSt2.lastPosDelta);

		return (objSt1.lastSent < objSt2.lastSent);
	}
};

#endif
