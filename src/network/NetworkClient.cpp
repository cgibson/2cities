#include "NetworkClient.h"

NetworkClient::NetworkClient() {
	//initialize();
}

NetworkClient::~NetworkClient() {}

void NetworkClient::initialize() {
	printf("Network Initialized\n");

	physicsEngine.initPhysics();
	physicsEngine.loadFromFile("resources/test.lvl");
	printf("Network Initiated Level in PhysicsEngine\n");
}

void NetworkClient::update(long milli_time) {
	// Check for Packet

	// TODO // REMOVE TEMP INCLUDED CODE
	physicsEngine.update(milli_time);

	InGameState *currState = global::stateManager->currentState;

	std::vector<WorldObject> physobjs = physicsEngine.getWorldObjects();
	currState->objects.clear();

	for(int i=0; i < physobjs.size(); i++) {
		currState->objects.push_back(new WorldObject(physobjs[i]));
		if (currState->objects[i]->getVelocity().mag() > 0) {
			printf("Item %i is moving item with vel=%s pos=%s\n",i,
					currState->objects[i]->getVelocity().str(),
					currState->objects[i]->getPosition().str());
		}
	}

	printf("Network Updated %i items!\n\n", physobjs.size());
}

void NetworkClient::addObject(WorldObject newObj) {
	physicsEngine.addWorldObject(newObj);
}

void NetworkClient::addObject(AmmoUnit newObj) {
	physicsEngine.addAmmo(newObj);
}

void NetworkClient::sendObject(enum E_WorldObjType newObjType, WorldObject newObj) {
	//physicsEngine.addWorldObject(newObj);
}
