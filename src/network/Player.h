#include "../../include/ting/Socket.hpp"

enum E_PlayerType {
	PLAYER, SPECTATOR
};

struct Player {
	int ID;
	int playerReady;
	int playerScore;
	Vector camPos;
	Vector camView;

	char *playerName;
	E_PlayerType playerType;

	ting::UDPSocket socket;
	ting::IPAddress ip;
	int lagDelay;
};
