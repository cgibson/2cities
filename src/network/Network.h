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

#include "../system/global.h"
#include "../system/enum.h"
using namespace global;
using namespace enumeration;

namespace Network {
	typedef enum N_PacketType {
		UNKNOWN, CONN_REQ, CONN_REPLY, DISCONNECT, STATUS_REQ, TEXT_MSG, OBJECT_REQ, OBJECT_SEND
	} PacketType;

	// A separate class to make the header distinct
	class NetworkPacketHeader {
	public:
		Network::N_PacketType type;
	};

	class NetworkPacket {
	public:
		NetworkPacketHeader header;
		unsigned char* data;
		unsigned int   dataSize;

		NetworkPacket() {
			dataSize = 0;
			data = NULL;
			header.type = UNKNOWN;
		}

		NetworkPacket(ting::Buffer<unsigned char> *buf, unsigned int recvSize) {
			dataSize = recvSize - sizeof(NetworkPacketHeader);
			data = new unsigned char[dataSize];
			memcpy(&header, buf->Buf(), sizeof(NetworkPacketHeader));
			memcpy(data, buf->Buf() + sizeof(NetworkPacketHeader), dataSize);
		}

		NetworkPacket(N_PacketType newType, unsigned char *newData, unsigned int newDataSize) {
			dataSize = newDataSize;
			data = new unsigned char[dataSize];
			header.type = newType;
			memcpy(data, newData, dataSize);
		}

		~NetworkPacket() {
			//delete [] data;
		}

		NetworkPacket operator=( const NetworkPacket& pkt) {
			this->header.type = pkt.header.type;
			this->dataSize = pkt.dataSize;

			this->data = new unsigned char[this->dataSize];
			memcpy(this->data, pkt.data, this->dataSize);
			return(*this );
		}

		void display() {
			printf("PacketDisplay> ");
			switch (header.type) {
			case CONN_REQ :
				printf("CONN_REQ\n");
				break;
			case CONN_REPLY :
				printf("CONN_REPLY: playerID=%i\n", *(int*)(data));
				break;
			case DISCONNECT :
				printf("DISCONNECT\n");
				break;
			case TEXT_MSG :
				printf("TEXT_MSG: %s\n", data);
				break;
			case OBJECT_REQ :
				printf("OBJECT_REQ: objectID=%i\n", *(int*)(data));
				break;
			case OBJECT_SEND :
				printf("OBJECT_SEND: objectID=%i\n", ((WorldObject*)data)->getID());
				break;
			default:
				printf("No Print Rule for Header Type! EMUM=%i\n",(int)(header.type));
				break;
			}
		}
	};
}

#endif