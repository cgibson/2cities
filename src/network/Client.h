#include "../../include/ting/Socket.hpp"

class Client {
	public:
		typedef enum E_PlayerType {
			PLAYER, SPECTATOR
		} PlayerType;

		int playerID;
		int playerReady;
		int playerScore;
		Vector camPos;
		Vector camView;

		char playerName[16];
		PlayerType playerType;

		bool isLocal;
		ting::UDPSocket socket;
		ting::IPAddress ip;
		int playerDelay;

		Client(int newPlayerID=0, char *newName=NULL, bool localPlayer=false) {
			playerID = newPlayerID;
			playerReady = 0;
			playerScore = 0;
			camPos      = Vector( 0,100, 0);
			camView     = Vector( 0,  0, 0);

			if(newName == NULL)
				playerName[0] = '\0';
			else
				strcpy(playerName, newName);

			playerType  = SPECTATOR;
			isLocal = localPlayer;
			playerDelay    = 0;
		}

		void print() {
			printf("*** Client Details ***\n");
			printf("playerID    = %i\n", playerID);
			printf("playerName  = %s\n", playerName);

			if(playerType == PLAYER)
				printf("playerType  = PLAYER\n");
			else
				printf("playerType  = SPECTATOR\n");

			printf("playerReady = %i\n", playerReady);
			printf("playerScore = %i\n", playerScore);
			printf("Camera Pos  = %s\n", camPos.str());
			printf("Camera View = %s\n", camView.str());
			printf("playerDelay = %i\n", playerDelay);
			printf("**********************\n");
		}

		int  makeClientBinStream(unsigned char *bufPtr) {
			int currPos = 0;
			int currISize;

			currISize = sizeof(PlayerType);
			memcpy(bufPtr + currPos, &playerType, currISize); currPos += currISize;

			if(playerType != PLAYER)
				return currPos;

			// WRITE playerID, Ready, Score, camPos, camView, Name
			currISize = sizeof(int);
			memcpy(bufPtr + currPos, &playerID   , currISize); currPos += currISize;
			memcpy(bufPtr + currPos, &playerReady, currISize); currPos += currISize;
			memcpy(bufPtr + currPos, &playerScore, currISize); currPos += currISize;

			double tmpD;
			currISize = sizeof(double);
			tmpD = camPos.x(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
			tmpD = camPos.y(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
			tmpD = camPos.z(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
			tmpD = camView.x(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
			tmpD = camView.y(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;
			tmpD = camView.z(); memcpy(bufPtr + currPos, &tmpD, currISize); currPos += currISize;

			currISize = sizeof(playerName);
			memcpy(bufPtr + currPos, playerName, currISize); currPos += currISize;

			return currPos;
		}

		int  recvClientBinStream(unsigned char *bufPtr) {
			int currPos = 0;
			int currISize;

			currISize = sizeof(PlayerType);
			memcpy(&playerType, bufPtr + currPos, currISize); currPos += currISize;

			if(playerType != PLAYER)
				return currPos;

			// WRITE playerID, Ready, Score, camPos, camView, Name
			currISize = sizeof(int);
			memcpy(&playerID   , bufPtr + currPos, currISize); currPos += currISize;
			memcpy(&playerReady, bufPtr + currPos, currISize); currPos += currISize;
			memcpy(&playerScore, bufPtr + currPos, currISize); currPos += currISize;

			double tmpD;
			currISize = sizeof(double);
			memcpy(&tmpD, bufPtr + currPos, currISize); camPos.x(tmpD); currPos += currISize;
			memcpy(&tmpD, bufPtr + currPos, currISize); camPos.y(tmpD); currPos += currISize;
			memcpy(&tmpD, bufPtr + currPos, currISize); camPos.z(tmpD); currPos += currISize;
			memcpy(&tmpD, bufPtr + currPos, currISize); camView.x(tmpD); currPos += currISize;
			memcpy(&tmpD, bufPtr + currPos, currISize); camView.y(tmpD); currPos += currISize;
			memcpy(&tmpD, bufPtr + currPos, currISize); camView.z(tmpD); currPos += currISize;

			currISize = sizeof(playerName);
			memcpy(playerName, bufPtr + currPos, currISize); currPos += currISize;

			return currPos;
		}

		static int  makeClientVectorBinStream(vector<Client *> &clientVec, unsigned char *bufPtr) {
			int currPos = 0;

			for(unsigned int i=0; i<clientVec.size(); ++i) {
				currPos += clientVec[i]->makeClientBinStream(bufPtr + currPos);
			}

			return currPos;
		}

		static int  recvClientVectorBinStream(vector<Client *> &clientVec, unsigned char *bufPtr, int bufSize) {
			int currPos = 0;
			unsigned int currVecLoc = 0;

			while(currPos < bufSize) {
				if(currVecLoc == clientVec.size())
					clientVec.push_back(new Client());

				currPos += clientVec[currVecLoc]->recvClientBinStream(bufPtr + currPos);
				currVecLoc++;
			}

			return currPos;
		}
};
