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
		CONN_REQ, CONN_REPLY, SERVER_UPD, CLIENT_SEND
	} PacketType;

	class NetworkPacketHeader {
	public:
		Network::N_PacketType type;
	};

	class NetworkPacket {
	public:
		Network::NetworkPacketHeader header;
		unsigned char* data;
		unsigned int   dataSize;

		NetworkPacket() {};
		NetworkPacket(N_PacketType newType, unsigned char * newData, unsigned int newDataSize) {
			header.type = newType;
			dataSize = newDataSize;
			data = new unsigned char[dataSize];
			memcpy(data, newData, dataSize);
		}
		~NetworkPacket() {
			delete data;
		}
	};
}

using namespace Network;

class NetworkSystem {
protected:
	void updateLocalObject(WorldObject *obj);

	ting::Buffer<ting::u8>* BuildBuffer(NetworkPacket* packet);
	NetworkPacket* ReadBuffer(ting::Buffer<ting::u8>* buf, unsigned int recvSize);

	void SendPacket(ting::UDPSocket *socket, ting::IPAddress *serverIP, NetworkPacket* pktPtr);
	NetworkPacket* RecvPacket(ting::UDPSocket *socket, ting::IPAddress *sourceIP);

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

#endif
