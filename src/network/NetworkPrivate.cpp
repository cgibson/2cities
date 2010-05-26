#include "NetworkPrivate.h"

NetworkPrivate::NetworkPrivate() {
	PRINTINFO("Network Initializing...");
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
	std::vector<WorldObject> PhysEngObjs = physicsEngine.getWorldObjects();
	for(unsigned int i=0; i < PhysEngObjs.size(); i++) {
		updateObjectLocal(new WorldObject(PhysEngObjs[i]));
	}
}

void NetworkPrivate::addObject(WorldObject newObj) {
	physicsEngine.addWorldObject(newObj);
}

void NetworkPrivate::loadLevel(const char * file) {
	// Clear GameState objects
	global::stateManager->currentState->objects.clear();

	physicsEngine.loadFromFile(file);
}
