#include "NetworkManager.h"

#ifdef CLIENT
	#include "../graphics/graphics.h"
#endif

#include "NetworkSystem.h"
#include "NetworkPrivate.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

NetworkManager::NetworkManager() {
	printf("NetworkManager Initializing...\n");

#ifdef CLIENT
//	network = new NetworkPrivate();
	network = new NetworkClient();
#else
	network = new NetworkServer();
#endif
	printf("NetworkManager Initialized\n");
}

void NetworkManager::initialize() {
#ifdef CLIENT
	gfx::hud.console.registerCmd("connect", NetworkManager::consoleCmds);
	gfx::hud.console.registerCmd("network", NetworkManager::consoleCmds);
	gfx::hud.console.registerCmd("msg", NetworkManager::consoleCmds);
	gfx::hud.console.registerCmd("disconnect", NetworkManager::consoleCmds);
	gfx::hud.console.registerCmd("cl", NetworkManager::consoleCmds); // TODO DEBUG REMOVE
#endif
}

void NetworkManager::changeNetworkInterface(E_NetworkInterface networkType) {
	NetworkSystem *oldNetwork = network;

	switch (networkType) {
#ifdef CLIENT
	case N_PRIVATE :
		network = new NetworkPrivate();
		break;
	case N_CLIENT :
		network = new NetworkClient();
		break;
#endif
	case N_SERVER :
		network = new NetworkServer();
		break;
	default:
		break;
	}
}

void NetworkManager::consoleCmds(int argc, char *argv[]) {
#ifdef CLIENT
	// TODO DEBUG REMOVE
	if(!strcmp(argv[0],"cl")) {
		networkManager->network->connectServer("127.0.0.1", 5060);
	}

	if(!strcmp(argv[0],"msg")) {
		int totalSize = 0;
		for(int i=1;i<argc;i++)
			totalSize += strlen(argv[i]) + 1;

		char msg[totalSize+1];
		int currPos = 0;
		for(int i=1;i<argc;i++) {
			strcpy(msg+currPos, argv[i]);
			currPos += strlen(argv[i]);
			msg[currPos] = ' ';
			currPos++;
		}
		msg[currPos-1] = '\0';
		printf("Sent> '%s'\n",msg);
		networkManager->network->sendMsg(msg);
	}

	if(!strcmp(argv[0],"disconnect")) {

	}

	if(!strcmp(argv[0],"connect")) {
		int results = 0;
		if (!strcmp(argv[1],"local")) {
			results = networkManager->network->connectServer("127.0.0.1", 5060);
		}
		else if (argc != 3) {
			gfx::hud.console.error("Usage: %s <host ip> <host port>", argv[0]);
			return;
		}
		else {
			results = networkManager->network->connectServer(argv[1], atoi(argv[2]));
		}

		if(results)
			gfx::hud.console.info("Connection Successful!");
		else
			gfx::hud.console.error("Connection Failure!");

		return;
	}

	if(!strcmp(argv[0],"network")) {
		if (argc != 2) {
			gfx::hud.console.error("Usage: %s <private | client | server>", argv[0]);
			return;
		}

		if(!strcmp(argv[1],"private")) {
			networkManager->changeNetworkInterface(N_PRIVATE);
			networkManager->network->loadLevel("resources/test.lvl");
		}
		else if(!strcmp(argv[1],"client")) {
			networkManager->changeNetworkInterface(N_CLIENT);
		}
		else if(!strcmp(argv[1],"server")) {
			networkManager->changeNetworkInterface(N_SERVER);
			networkManager->network->loadLevel("resources/test.lvl");
		}
		else {
			gfx::hud.console.error("Usage: %s <private | client | server>", argv[0]);
			return;
		}

	}
#endif
}