#include "NetworkServer.h"

using namespace global;

NetworkServer::NetworkServer() {
	initialize();
}

NetworkServer::~NetworkServer() {}

void NetworkServer::initialize() {
	physics.initPhysics();
	physics.loadFromFile("resources/test.lvl");
}

void NetworkServer::update(long milli_time) {
	// CarnageState *state = (CarnageState*)global::stateManager.currentState;
	// state->Buildings = physics.getBuildingBlocks();
	// state->Ammo = physics.getAmmo();
}

void NetworkServer::addObject(WorldObject newObj) {
	//physics.addAmmo(newAmmo);
}
