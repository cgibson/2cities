#include "../../include/ting/Socket.hpp"

class Player {
public:
	int ID;
	char *playerName;

	ting::UDPSocket socket;
	ting::IPAddress ip;
};
