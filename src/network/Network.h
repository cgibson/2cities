#ifndef _NETWORK_H_
#define _NETWORK_H_

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

namespace net{
	enum N_PacketType {
		UNKNOWN,
		CONN_REQ, CONN_REPLY, DISCONNECT,
		LAG_REQ, LAG_REPLY, LAG_RESULT,
		STATUS_UPDATE, STATE_COUNTDOWN,
		CAMLOC_MYLOC, PLAYER_READY, PLAYER_NAME,
		SCORE_MINE, DAMAGE_MINE,
		LEVEL_LOAD, LEVEL_CLEAR, LEVEL_BATCHOBJ, TEXT_MSG,
		OBJECT_KILL, OBJECT_REQ, OBJECT_SEND, OBJECT_BATCHSEND
	};

	extern const unsigned int OBJECT_BATCHSEND_SIZE;

	extern unsigned int CLIENT_SEND_MAX_PACKETS_PER_CYCLE;
	extern unsigned int CLIENT_RECV_MAX_PACKETS_PER_CYCLE;

	extern unsigned int SERVER_SEND_MAX_PACKETS_PER_MS;
	extern unsigned int SERVER_SEND_MAX_PACKETS_PER_CYCLE;
	extern unsigned int SERVER_RECV_MAX_PACKETS_PER_CYCLE;

	extern unsigned int SERVER_PHYSICS_UPDATE_RATE;

	extern const unsigned int SERVER_PORT_DEFAULT;

	extern const int TIME_IN_BUILD_STATE;
	extern const int TIME_IN_CARNAGE_STATE;
	extern const int TIME_IN_RESULTS_STATE;

	extern const float OBJECT_KILL_DELTA;
	extern const int DAMAGE_PERCENT_WIN_CONDITION;

	// A separate class to make the header distinct
	struct NetworkPacketHeader {
		net::N_PacketType type;
	};

	class NetworkPacket {
	public:
		NetworkPacketHeader header;
		unsigned char data[2000];
		unsigned int  dataSize;

		NetworkPacket();
		NetworkPacket(ting::Buffer<unsigned char> *buf, unsigned int recvSize);
		NetworkPacket(N_PacketType newType, unsigned char *newData, unsigned int newDataSize);
		~NetworkPacket() {};

		NetworkPacket operator=( const NetworkPacket& pkt);

		void display();
	};
}

#endif
