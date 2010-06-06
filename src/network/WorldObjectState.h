#ifndef _WORLDOBJECTSTATE_H_
#define _WORLDOBJECTSTATE_H_

#include <math.h>
#include <vector>
#include <algorithm>

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
	int    lastSent;

	WorldObjectState(WorldObject * newObj, int initPriority = 2) {
		objPtr       = newObj;
		objID        = newObj->getID();
		velMag       = newObj->getForce().x();

		initPos      = newObj->getPosition();
		initPosDelta = 0;

		lastPos      = initPos;
		lastPosDelta = 0;

		lastUpdate   = newObj->getTimeStamp();
		lastSent     = newObj->getTimeStamp();
		priority     = initPriority;
	}

	void update(WorldObject * newObj) {
		// Don't need to import data as its already itself
		objPtr->import(*newObj);	// TODO breaks polymorphism
		lastUpdate   = objPtr->getTimeStamp();
			
		update();
	}

	void update() {
		if(objPtr != NULL) {
			velMag       = objPtr->getForce().x();
			initPosDelta = (objPtr->getPosition() - initPos).mag();
			lastPosDelta = (objPtr->getPosition() - lastPos).mag();
			lastPos      = objPtr->getPosition();
		}
	}

	void kill() {
		objPtr = NULL;
		priority = -1;
	}

	void sent(int currTime) {
		priority = std::max(0, priority - 1);
		lastSent = currTime;
	}

	static void updateVector(std::vector<WorldObjectState> *objVec, WorldObject *objPtr) {
		unsigned int i = 0;

		// *** Linear Search Code ***
		while (i < objVec->size() && (*objVec)[i].objID != objPtr->getID()) { i++; }

		// *** Add, Insert, Update Code ***
		// ObjID Not Found (and at end)... Push_back
		if (i == objVec->size()) {
			objVec->push_back(WorldObjectState(objPtr));
			//printf("Added Object\n");
		}
		// ObjID Found, Replace Data
		else {
			(*objVec)[i].update();
			//printf("Updated Object\n");
		}
	}

	static void removeVector(std::vector<WorldObjectState> *objVec, unsigned int objID) {
		unsigned int i=0;

		// *** Linear Search Code ***
		while (i < objVec->size() && (*objVec)[i].objID != objID) { i++; }

		// ObjID found, erase entry
		if (i < objVec->size()) {
			//(*objVec)[i].kill(); // TODO
			objVec->erase(objVec->begin()+i);
			//printf("Killed Object\n");
		}
	}

	static void sortVector(std::vector<WorldObjectState> *objVec) {
		//printf("Sorting Server Vector\n");
		std::sort((*objVec).begin(),(*objVec).end(), WorldObjectState::NetworkPrioritySort);
//		if((*objVec).size() > 0 && (*objVec)[0].priority > 0)
//			printf("Obj 0 priority\n", (*objVec)[0].priority);
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
