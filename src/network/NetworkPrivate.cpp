#include "NetworkPrivate.h"

NetworkPrivate::NetworkPrivate() {
	PRINTINFO("Network Initializing...");
	_playerID = 1;
	_currObjID = _playerID * 10000;
	PRINTINFO("Network Initialized!\n");

	PRINTINFO("Network Initializing PhysicsEngine...");
	physicsEngine.initPhysics();
	PRINTINFO("PhysicsEngine Initialized!\n");
}

NetworkPrivate::~NetworkPrivate() {}

void NetworkPrivate::initialize() {}

void NetworkPrivate::update(long milli_time) {
	// Update PhysicsEngine
	physicsEngine.update(milli_time);

	// Update Current Gamestate
	std::vector<WorldObject *> PhysEngObjs = physicsEngine.getWorldObjects();

/*
	std::vector<WorldObject *> PhysEngObjPtrs;
	for(unsigned int i=0; i < PhysEngObjs.size(); i++) {
		PhysEngObjPtrs.push_back(new WorldObject(PhysEngObjs[i]));
	}
	global::stateManager->currentState->objects.swap(PhysEngObjPtrs);
*/
	for(unsigned int i=0; i < PhysEngObjs.size(); i++) {
		if(PhysEngObjs[i]->getPosition().y() < -50.0f) {
			unsigned int removeID = PhysEngObjs[i]->getID();
			// Local Removal
			physicsEngine.removeWorldObject(removeID);
			removeObjectLocal(removeID);
		}
		else {
			updateObjectLocal(new WorldObject(*PhysEngObjs[i]));
		}
	}
}

void NetworkPrivate::addObject(WorldObject *newObj) {
	newObj->setID(_currObjID++);
	physicsEngine.addWorldObject(*newObj);
}

void NetworkPrivate::loadLevel(const char * file) {
	// Clear GameState objects
	global::stateManager->currentState->objects.clear();

	physicsEngine.loadFromFile(file);
}
