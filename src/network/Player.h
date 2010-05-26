#include "../../include/ting/Socket.hpp"

struct Player {
	int ID;
	char *playerName;

	ting::UDPSocket socket;
	ting::IPAddress ip;

	int lagDelay;
};
