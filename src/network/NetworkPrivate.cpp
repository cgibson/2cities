#include "NetworkPrivate.h"

NetworkPrivate::NetworkPrivate() {
	_playerID = 0;

	// TODO replace with console output
	printf("Network Initialized\n");

	physicsEngine = new Physics();
	physicsEngine->initPhysics();
	// TODO replace with console output
	printf("Network Initiated PhysicsEngine\n");
}

NetworkPrivate::~NetworkPrivate() {}

void NetworkPrivate::initialize() {}

void NetworkPrivate::update(long milli_time) {
	physicsEngine->update(milli_time);

	InGameState *currState = global::stateManager->currentState;

	std::vector<WorldObject> PhysEngObjs = physicsEngine->getWorldObjects();
	currState->objects.clear();

	for(int i=0; i < PhysEngObjs.size(); i++) {
		currState->objects.push_back(new WorldObject(PhysEngObjs[i]));
		if (currState->objects[i]->getVelocity().mag() > 0) {
			/*
			printf("Item %i is moving item with vel=%s pos=%s\n",i,
					currState->objects[i]->getVelocity().str(),
					currState->objects[i]->getPosition().str());
			*/
		}
	}
	/*
	printf("Physics Objects: WorldObjects=%i, BuildingBlocks=%i, AmmoUnits=%i\n",
			physicsEngine->getWorldObjects().size(),
			physicsEngine->getBuildingBlocks().size(),
			physicsEngine->getAmmo().size());
	*/
}

void NetworkPrivate::addObject(WorldObject newObj) {
	physicsEngine->addWorldObject(newObj);
}

void NetworkPrivate::loadLevel(const char * file) {
	physicsEngine->loadFromFile(file);
}