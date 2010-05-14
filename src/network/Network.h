#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "NetworkPrivate.h"
#include "NetworkClient.h"
#include "NetworkServer.h"

namespace NetworkSys {
	void switchNetwork(E_NetworkInterface networkType) {
		NetworkSystem *newNetwork;

		switch (networkType) {
		case N_PRIVATE :
			newNetwork = (NetworkSystem*)(new NetworkPrivate());
			break;
		case N_CLIENT :
			newNetwork = (NetworkSystem*)(new NetworkClient());
			break;
		case N_SERVER :
			newNetwork = (NetworkSystem*)(new NetworkServer());
			break;
		default:
			break;
		}

		NetworkSystem *oldNetwork = network;
		network = newNetwork;
		delete oldNetwork;
	}



}

#endif
