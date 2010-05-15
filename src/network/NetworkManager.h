#ifndef _NETWORKMANAGER_H_
#define _NETWORKMANAGER_H_

#include "../system/enum.h"
using namespace enumeration;

class NetworkSystem;

class NetworkManager {
public:
	NetworkSystem *network;

	NetworkManager();
	~NetworkManager() {};

	void initialize();

	void changeNetworkInterface(E_NetworkInterface networkType);

	static void consoleCmds(int argc, char *argv[]);
};

#include "NetworkSystem.h"

#endif
