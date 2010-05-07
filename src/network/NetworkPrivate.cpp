#include "NetworkPrivate.h"

NetworkPrivate::NetworkPrivate() {
	_playerID = 0;

	// TODO replace with console output
	printf("Network Initialized\n");

	physicsEngine = new Physics();
	physicsEngine->initPhysics();
	physicsEngine->loadFromFile("resources/test.lvl");
	// TODO replace with console output
	printf("Network Initiated Level in PhysicsEngine\n");
}

NetworkPrivate::~NetworkPrivate() {}

void NetworkPrivate::initialize() {}

void NetworkPrivate::update(long milli_time) {
	physicsEngine->update(milli_time);

	InGameState *currState = global::stateManager->currentState;

	std::vector<WorldObject> physobjs = physicsEngine->getWorldObjects();
	currState->objects.clear();

	for(int i=0; i < physobjs.size(); i++) {
		currState->objects.push_back(new WorldObject(physobjs[i]));
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

void NetworkPrivate::addObject(AmmoUnit newObj) {
	physicsEngine->addAmmo(newObj);
}
