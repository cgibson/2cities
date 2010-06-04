#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "global.h"
#include "../system/enum.h"

using namespace std;
using namespace enumeration;

E_STATE beginState;

void initState() {
	global::stateManager->changeCurrentState(beginState);
}

void initialize() {
 	// initialization of all other modules
	initState();
	networkManager->initialize();
}

/*
 * application kickstart.
 */
int main(int argc, char** argv) {
	if(argc == 2) {
		if(!strcmp(argv[1], "-build")) {
		  beginState=BUILD_STATE;
		}
	}
	else if(argc == 2) {
		if(!strcmp(argv[1], "-carnage")) {
		  beginState=CARNAGE_STATE;
		}
	}
	else {
		beginState=BUILD_STATE;
	}

	initialize();

	while (1)
		networkManager->network->update(-1);

	return 0;
}
