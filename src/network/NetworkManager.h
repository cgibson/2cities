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

	static void consoleConnect(int argc, char *argv[]);
	static void consoleDisconnect(int argc, char *argv[]);
	static void consoleChangeInterface(int argc, char *argv[]);
	static void consoleSendMsg(int argc, char *argv[]);
	static void consoleChangeLevel(int argc, char *argv[]);

	static void consoleNetworkRates(int argc, char *argv[]);
	static void consoleNetworkTests(int argc, char *argv[]);
};

#include "NetworkSystem.h"

#endif
