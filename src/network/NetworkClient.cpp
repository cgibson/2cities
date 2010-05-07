#include "NetworkClient.h"

NetworkClient::NetworkClient() {}

NetworkClient::~NetworkClient() {}

void NetworkClient::initialize() {
	_playerID = 0;

	// TODO replace with console output
	printf("Network Initialized\n");
}

void NetworkClient::update(long milli_time) {
	// TODO GET NETWORK DATA

	// TODO UPDATE LOCAL DATA
}

void NetworkClient::connectServer(const char * ip, unsigned int port) {

}

void NetworkClient::addObject(WorldObject newObj) {
	physicsEngine->addWorldObject(newObj);
}
