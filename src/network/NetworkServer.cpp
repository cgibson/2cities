#include "NetworkServer.h"

NetworkServer::NetworkServer() {
	_playerID = 1;

	// TODO replace with console output
	printf("Network Initialized\n");

	physicsEngine->initPhysics();
	physicsEngine->loadFromFile("resources/test.lvl");
	// TODO replace with console output
	printf("Network Initiated Level in PhysicsEngine\n");
}

NetworkServer::~NetworkServer() {}

void NetworkServer::initialize() {}

void NetworkServer::update(long milli_time) {
	physicsEngine->update(milli_time);
	// UPDATE DELTAs

	// UPDATE CLIENT DATA

	// UPDATE LOCAL DATA
	InGameState *currState = global::stateManager->currentState;
	std::vector<WorldObject> PhysEngObjs = physicsEngine->getWorldObjects();
	currState->objects.clear();

	for(int i=0; i < PhysEngObjs.size(); i++) {
		currState->objects.push_back(new WorldObject(PhysEngObjs[i]));
		if (currState->objects[i]->getVelocity().mag() > 0) {
			printf("Item %i is moving item with vel=%s pos=%s\n",i,
					currState->objects[i]->getVelocity().str(),
					currState->objects[i]->getPosition().str());
		}
	}
}

void NetworkServer::addObject(WorldObject newObj) {
	physicsEngine->addWorldObject(newObj);
}
