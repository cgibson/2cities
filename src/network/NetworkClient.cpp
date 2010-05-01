#include "NetworkClient.h"
#include "../system/global.h"
#include "../state/PhysObj.h"
#include "../state/CarnageState.h"

using namespace global;

NetworkClient::NetworkClient() {
	initialize();
}

NetworkClient::~NetworkClient() {}

void NetworkClient::initialize() {

}

void NetworkClient::update(long milli_time) {
	//CarnageState *state = (CarnageState*)global::stateManager.currentState;
	//state->Buildings = physics.getBuildingBlocks();
}

void NetworkClient::addObject(PhysObj newObj) {
	// send request to server to add object
}
