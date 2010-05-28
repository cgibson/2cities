#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "global.h"
#include "../state/CarnageState.h"
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
	else {
		beginState=CARNAGE_STATE;
	}

	initialize();
	networkManager->network->update(0);

	int timePrev = global::elapsed_ms();
	int timeCurr;
	int elapsed = 0;
	while(1) {
		//while((timePrev + 1) > (timeCurr = global::elapsed_ms()));
		//elapsed = (timeCurr - timePrev);
		//timePrev = timeCurr;
		//networkManager->network->update(elapsed);
		networkManager->network->update(-1);
	}


	/*
	char input[100];
	while(1) {
		cout << "Enter Command:" << endl;
		cin >> input;

		if(!strcmp(input, "objects")) {
			cout << "*** Printing Objects ***" << endl;
			for(int i=0; i < stateManager->currentState->objects.size(); i++)
				cout << "Obj #" << i << ": " << stateManager->currentState->objects[i]->getPosition().str() << endl;
			cout  << "*** Objects Completed ***" << endl;
		}
		if(!strcmp(input, "newObject")) {
			Vector dir(1,0,0);
			dir.norm();

			// TODO uniqueID creation & addObject(WorldObject)
			static int newObjID = 10000;
			WorldObject newObj = WorldObject(newObjID++,0,enumeration::DUMMY_SPHERE);
			newObj.setPosition(Vector(0,10,0));
			newObj.setVelocity(dir * 50);
			network->addObject(newObj);
		}
		if(!strcmp(input, "newLevel")) {
			network->loadLevel("resources/test.lvl");
		}

		network->update(100);
	}
	*/


	return 0;
}
