#include "NetworkClient.h"

using namespace global;

NetworkClient::NetworkClient() {
	initialize();
}

NetworkClient::~NetworkClient() {}

void NetworkClient::initialize() {
	physicsEngine.initPhysics();
	physicsEngine.loadFromFile("resources/test.lvl");
	printf("Physics initialized\n");
}

void NetworkClient::update(long milli_time) {
	// Check for Packet

	/*
	// TODO // REMOVE TEMP INCLUDED CODE
	physicsEngine.update(milli_time);
	std::vector<BuildingUnit> physicsObjects = physicsEngine.getBuildingBlocks();
	std::vector<WorldObject> worldObjs;
	WorldObject worldObj;
	for(int i=0; i<physicsObjects.size(); i++) {
		worldObj = (WorldObject)physicsObjects[i];
		worldObjs.push_back(worldObj);
	}
	((CarnageState*)global::stateManager.currentState)->objects = worldObjs;
	*/
}

void NetworkClient::addObject(enum E_WorldObjType newObjType, WorldObject newObj) {
	// Send Enum to Server
	sendObject(newObjType, newObj);
}

void NetworkClient::sendObject(enum E_WorldObjType newObjType, WorldObject newObj) {
//	BuildingUnit tempObj = (BuildingUnit)newObj;
//	physicsEngine.addBuildingBlock(tempObj);
}
