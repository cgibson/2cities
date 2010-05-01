#include <map>

#include "NetworkServer.h"
#include "../system/global.h"
#include "../state/PhysObj.h"
#include "../state/CarnageState.h"

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

void NetworkServer::addObject(PhysObj newObj) {
	//physics.addAmmo(newAmmo);
}
