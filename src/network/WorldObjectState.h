#ifndef _WORLDOBJECTSTATE_H_
#define _WORLDOBJECTSTATE_H_

#include <math.h>
#include <vector>

class WorldObjectState {
public:
	WorldObject *objPtr;

	int objID;
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

	WorldObjectState(WorldObject * newObj, int initPriority = 2) {
		objPtr       = newObj;
		objID        = newObj->getID();
		velMag       = newObj->getForce().x();

		initPos      = newObj->getPosition();
		initPosDelta = 0;

		lastPos      = initPos;
		lastPosDelta = 0;

		lastUpdate   = newObj->getTimeStamp();
		priority     = initPriority;
	}

	void update(WorldObject * newObj) {
		// Don't need to import data as its already itself
		objPtr->import(*newObj);	// TODO breaks polymorphism

		velMag       = newObj->getForce().x();
		initPosDelta = (newObj->getPosition() - initPos).mag();
		lastPosDelta = (newObj->getPosition() - lastPos).mag();
		lastPos      = newObj->getPosition();
		lastUpdate   = newObj->getTimeStamp();
		priority     = std::max(0, priority - 1);
	}

	void update() {
		if(objPtr != NULL) {
			velMag       = objPtr->getForce().x();
			initPosDelta = (objPtr->getPosition() - initPos).mag();
			lastPosDelta = (objPtr->getPosition() - lastPos).mag();
			lastPos      = objPtr->getPosition();
			lastUpdate   = objPtr->getTimeStamp();
			priority     = std::max(0, priority - 1);
		}
	}

	void kill() {
		objPtr = NULL;
		priority = -1;
	}

	static void updateVector(std::vector<WorldObjectState> *objVec, WorldObjectState *objPtr) {
		unsigned int i = 0;

		// *** Linear Search Code ***
		while (i < objVec->size() && (*objVec)[i].objID != objPtr->objID) { i++; }

		// *** Add, Insert, Update Code ***
		// ObjID Not Found (and at end)... Push_back
		if (i == objVec->size()) {
			objVec->push_back(*objPtr);
		}
		// ObjID Found, Replace Data
		else {
			(*objVec)[i].update();
		}
	}

	static void removeVector(std::vector<WorldObjectState> *objVec, unsigned int objID) {
		unsigned int i=0;

		// *** Linear Search Code ***
		while (i < objVec->size() && (*objVec)[i].objID != objID) { i++; }

		// ObjID found, erase entry
		if (i < objVec->size()) {
			(*objVec)[i].kill();
		}
	}

	static bool NetworkPrioritySort(WorldObjectState objSt1, WorldObjectState objSt2) {
		if(objSt1.priority != objSt2.priority)
			return (objSt1.priority > objSt2.priority);

		if(abs(objSt1.velMag - objSt2.velMag) > 0.5f) //(5*net::timeElapsed/1000.0f))
			return (objSt1.velMag > objSt2.velMag);

		if(abs(objSt1.lastPosDelta - objSt2.lastPosDelta) > 0.5f) //(5*net::timeElapsed/1000.0f))
			return (objSt1.lastPosDelta > objSt2.lastPosDelta);

		return (objSt1.lastUpdate < objSt2.lastUpdate);
	}
};

#endif
