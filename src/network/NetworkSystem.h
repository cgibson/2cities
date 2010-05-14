#ifndef _NETWORKSYSTEM_H_
#define _NETWORKSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"

#include "../../include/ting/Socket.hpp"
#include "../../include/ting/WaitSet.hpp"

#ifdef SERVER
	#define PRINTINFO(x) printf(x);
#else
	//#include "../graphics/graphics.h"
	//#define PRINTINFO(x) gfx::hud.console.info(x);
	#define PRINTINFO(x) printf(x);
#endif

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

namespace Network {
	typedef enum N_PacketType {
		CONN_REQ, CONN_REP, SERVER_UPD, CLIENT_SEND
	} PacketType;

	class NetworkPacketHeader {
	public:
		Network::N_PacketType type;
	};

	class NetworkPacket {
	public:
		Network::NetworkPacketHeader header;
		unsigned char* data;
		int dataSize;
	};
}

class NetworkSystem {
protected:
	void updateLocalObject(WorldObject *obj);

	ting::Buffer<ting::u8>* BuildBuffer(Network::NetworkPacket* packet);
	Network::NetworkPacket* ReadBuffer(ting::Buffer<ting::u8>* buf);

public:
	NetworkSystem() {}
	~NetworkSystem() {};

	virtual void initialize() {};
	virtual void update(long milli_time) {};

	virtual bool connectServer(const char * ip, unsigned int port) { return false; };

	// Add new object to scene
	virtual void addObject(WorldObject newObj) {};

	// Load a stored lvl
	virtual void loadLevel(const char * file) {};

	virtual void reqUpdateObj(unsigned int objID) {};
};

// Method to take a WorldObject* and update/add it to the main vector (based on ID field)
// ** NOTE: Current VERY inefficient **
// TODO Improve efficiency
inline void NetworkSystem::updateLocalObject(WorldObject *obj) {
	int i=0;

	std::vector<WorldObject *> *currObjects = &(global::stateManager->currentState->objects);
	while (i < currObjects->size() && (*currObjects)[i]->getID() != obj->getID())
		i++;

	if (i == currObjects->size())
		currObjects->push_back(obj);
	else
		(*currObjects)[i] = obj;
}

inline ting::Buffer<ting::u8>* NetworkSystem::BuildBuffer(Network::NetworkPacket* packet) {
	int bufSize = packet->dataSize + sizeof(packet->header);
	ting::Buffer<ting::u8> *buf = new ting::Buffer<ting::u8>(new ting::u8[bufSize], bufSize);

	memcpy(buf->Begin(), &packet->header, sizeof(packet->header));
	memcpy(buf->Begin() + sizeof(packet->header), packet->data, packet->dataSize);

	return buf;
}

inline Network::NetworkPacket* NetworkSystem::ReadBuffer(ting::Buffer<ting::u8>* buf) {
	Network::NetworkPacket* packet = new Network::NetworkPacket;

	memcpy(&packet->header, buf->Begin(), sizeof(&packet->header));

	packet->dataSize = buf->Size() - sizeof(&packet->header);
	memcpy(&packet->data, buf->Begin() + sizeof(&packet->header), packet->dataSize);

	return packet;
}

#endif
