#include "Network.h"

#include "../system/global.h"
using namespace global;

namespace net{
	const unsigned int OBJECT_BATCHSEND_SIZE = 10;	// must ensure size * objsBinStream < 1400

	unsigned int CLIENT_SEND_MAX_PACKETS_PER_CYCLE = 10;
	unsigned int CLIENT_RECV_MAX_PACKETS_PER_CYCLE = 200;

	unsigned int SERVER_SEND_MAX_PACKETS_PER_MS = 1;
	unsigned int SERVER_SEND_MAX_PACKETS_PER_CYCLE = 40;
	unsigned int SERVER_RECV_MAX_PACKETS_PER_CYCLE = 15;

	unsigned int SERVER_PHYSICS_UPDATE_RATE = 1;

	const unsigned int SERVER_PORT_DEFAULT = 5060;

	const int TIME_IN_BUILD_STATE   = 9999; // in sec
	const int TIME_IN_CARNAGE_STATE = 300; // in sec
	const int TIME_IN_RESULTS_STATE = 30; // in sec

	const float OBJECT_KILL_DELTA = 7.0f;
	const int DAMAGE_PERCENT_WIN_CONDITION = 95;

	NetworkPacket::NetworkPacket() {
			dataSize = 0;
			//data = NULL;
			header.type = UNKNOWN;
	}

	NetworkPacket::NetworkPacket(ting::Buffer<unsigned char> *buf, unsigned int recvSize) {
		dataSize = recvSize - sizeof(NetworkPacketHeader);
		//data = new unsigned char[dataSize];
		memcpy(&header, buf->Buf(), sizeof(NetworkPacketHeader));
		memcpy(data, buf->Buf() + sizeof(NetworkPacketHeader), dataSize);
	}

	NetworkPacket::NetworkPacket(N_PacketType newType, unsigned char *newData, unsigned int newDataSize) {
		dataSize = newDataSize;
		//data = new unsigned char[dataSize];
		header.type = newType;
		memcpy(data, newData, dataSize);
	}

	NetworkPacket NetworkPacket::operator=( const NetworkPacket& pkt) {
		this->header.type = pkt.header.type;
		this->dataSize = pkt.dataSize;

		//this->data = new unsigned char[this->dataSize];
		memcpy(this->data, pkt.data, this->dataSize);
		return(*this );
	}

	void NetworkPacket::display() {
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
		case OBJECT_BATCHSEND :
			printf("OBJECT_BATCHSEND: IDs... ");
			// TODO print objects included in batch.
			for(int i=0; i<10; i++)
				printf("%i ",((WorldObject*)data + i)->getID());
			printf("\n");
			break;
		default:
			printf("No Print Rule for Header Type! EMUM=%i\n",(int)(header.type));
			break;
		}
	}
}
