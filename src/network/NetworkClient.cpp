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

bool NetworkClient::connectServer(const char * ip, unsigned int port) {

	return false;
}

void NetworkClient::addObject(WorldObject newObj) {

}
