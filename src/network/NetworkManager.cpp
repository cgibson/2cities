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
	gfx::hud.console.registerCmd("connect",    NetworkManager::consoleConnect);
	gfx::hud.console.registerCmd("cl",         NetworkManager::consoleConnect);
	gfx::hud.console.registerCmd("disconnect", NetworkManager::consoleDisconnect);

	gfx::hud.console.registerCmd("network",    NetworkManager::consoleChangeInterface);
	gfx::hud.console.registerCmd("n",          NetworkManager::consoleChangeInterface);

	gfx::hud.console.registerCmd("msg",        NetworkManager::consoleSendMsg);

	gfx::hud.console.registerCmd("loadlevel",  NetworkManager::consoleChangeLevel);
	gfx::hud.console.registerCmd("ll",         NetworkManager::consoleChangeLevel);

	gfx::hud.console.registerCmd("netrate",    NetworkManager::consoleNetworkRates);
	gfx::hud.console.registerCmd("nr",         NetworkManager::consoleNetworkRates);

	gfx::hud.console.registerCmd("nettest",    NetworkManager::consoleNetworkTests);
	gfx::hud.console.registerCmd("nt",         NetworkManager::consoleNetworkTests);
#endif
}

void NetworkManager::changeNetworkInterface(E_NetworkInterface networkType) {
	NetworkSystem *oldNetwork = network;
	network->closeSockets();

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
	delete oldNetwork;
}

void NetworkManager::consoleChangeLevel(int argc, char *argv[]) {
#ifdef CLIENT
	if (argc != 2) {
		gfx::hud.console.info("Usage: %s <level>. Loading Default: resource/test2.lvl", argv[0]);
		networkManager->network->loadLevel("resources/test2.lvl");
	}
	else {
		networkManager->network->loadLevel(argv[1]);
	}
#endif
}

void NetworkManager::consoleSendMsg(int argc, char *argv[]) {
#ifdef CLIENT
	int totalSize = 0;
	for(int i=1;i<argc;i++)
		totalSize += strlen(argv[i]) + 1; // +1 for space and later \0

	char msg[totalSize+1];
	int currPos = 0;
	for(int i=1;i<argc;i++) {
		strcpy(msg+currPos, argv[i]);
		currPos += strlen(argv[i]);
		msg[currPos++] = ' ';
	}
	msg[currPos-1] = '\0';
	printf("Sent> '%s'\n",msg);
	networkManager->network->sendMsg(msg);
#endif
}

void NetworkManager::consoleDisconnect(int argc, char *argv[]) {
#ifdef CLIENT
	networkManager->network->disconnectServer();
#endif
}

void NetworkManager::consoleConnect(int argc, char *argv[]) {
#ifdef CLIENT
	int results = 0;
	if (!strcmp(argv[1],"local") || !strcmp(argv[0],"cl")) {
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

#endif
}

void NetworkManager::consoleChangeInterface(int argc, char *argv[]) {
#ifdef CLIENT
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
	}
	else {
		gfx::hud.console.error("Usage: %s <private | client | server>", argv[0]);
		return;
	}
#endif
}

void NetworkManager::consoleNetworkRates(int argc, char *argv[]) {
#ifdef CLIENT
	if(argc != 3)
		return;

	if(!strcmp(argv[1],"physics")) {
		net::SERVER_PHYSICS_UPDATE_RATE = atoi(argv[2]);
		printf("SERVER PHYSICS RATE = %i\n", net::SERVER_PHYSICS_UPDATE_RATE);
	}
	else if(!strcmp(argv[1],"client_recv")) {
		net::CLIENT_RECV_MAX_PACKETS_PER_CYCLE = atoi(argv[2]);
		printf("CLIENT RECV MAX = %i\n", net::CLIENT_RECV_MAX_PACKETS_PER_CYCLE);
	}
	else if(!strcmp(argv[1],"server_recv")) {
		net::SERVER_SEND_MAX_PACKETS_PER_MS = atoi(argv[2]);
		printf("SERVER SEND MAX = %i\n", net::SERVER_SEND_MAX_PACKETS_PER_MS);
	}
	return;
#endif
}

void NetworkManager::consoleNetworkTests(int argc, char *argv[]) {
#ifdef CLIENT
	if(!strcmp(argv[1],"WorldObjects")) {
		WorldObject testObj(0,0, (enumeration::ObjectType)SMALL_SPHERE);
		testObj.print();

		unsigned char buf[500];
		int r = testObj.makeBinStream(buf);
		printf("buf used = %i\n", r);

		WorldObject testObj2(buf);
		testObj2.print();
	}
	if(!strcmp(argv[1],"oi")) {
		if(global::interpolateObjects) {
			global::interpolateObjects = false;
			gfx::hud.console.info("Object Interpolation Off");
		}
		else {
			global::interpolateObjects = true;
			gfx::hud.console.info("Object Interpolation On");
		}
	}
	if(!strcmp(argv[1],"s")) {
		stateManager->switchToCarnage();
	}
#endif
}
