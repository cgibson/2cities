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
			// TODO currently works due to deletes/update timing. However,
			// following command should be a new pointer of its own.
			updateObjectLocal(PhysEngObjs[i]);
		}
	}
}

/*******************************************
 * OBJECT/LEVEL/WORLD FUNCTIONS
 *******************************************/
void NetworkPrivate::addObject(WorldObject *newObj) {
	newObj->setID(nextNewObjID++);
	physicsEngine.addWorldObject(newObj);
}

void NetworkPrivate::emptyWorld() {
	// Clear GameState objects
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
