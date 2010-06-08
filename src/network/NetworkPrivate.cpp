#include "NetworkPrivate.h"

/*******************************************
 * CONSTRUCTORS / DESTRUCTORS
 *******************************************/
NetworkPrivate::NetworkPrivate() {
	PRINTINFO("Network Initializing...");

	myClientID = 0;
	char playerName[] = "Priv Player";	// TODO check pkt.data for name
	clients.push_back(new Client(1,playerName));

	nextNewObjID = clients[myClientID]->playerID * 10000;

	PRINTINFO("Network Initialized!\n");

	PRINTINFO("Network Initializing PhysicsEngine...");
	physicsEngine.initPhysics();
	PRINTINFO("PhysicsEngine Initialized!\n");
}

NetworkPrivate::~NetworkPrivate() {}

/*******************************************
 * CORE FUNCTIONS
 *******************************************/

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
			WorldObject * tmpObj = global::factory->makeObject(PhysEngObjs[i]->getType());
			tmpObj->import(PhysEngObjs[i]);
			updateObjectLocal(tmpObj);
		}
	}
}

/*******************************************
 * OBJECT/LEVEL/WORLD FUNCTIONS
 *******************************************/
void NetworkPrivate::addObject(WorldObject *newObj, int newID) {
	if(newID == -1)
		newObj->setID(nextNewObjID++);
	else
		newObj->setID(newID);

	addObjectPhys(newObj);
}

void NetworkPrivate::addObjectPhys(WorldObject *objPtr) {
	objPtr->setTimeStamp(global::elapsed_ms());

	physicsEngine.addWorldObject(objPtr);
}

void NetworkPrivate::emptyWorld() {
	// Clear GameState objects
	// Note: clear okay since Physics will do actual WO deletes
	global::stateManager->currentState->objects.clear();

	// Clear Physics
	physicsEngine.emptyWorld();
}

void NetworkPrivate::loadLevel(const char * file) {
	emptyWorld();

	physicsEngine.loadFromFile(file);
}

/*******************************************
 * PLAYER INTERACTION FUNCTIONS
 *******************************************/
void NetworkPrivate::setPlayerReady(int readyFlag) {
	global::stateManager->switchToCarnage();	// TODO REMOVE. TEMP FLOW CODE
}
