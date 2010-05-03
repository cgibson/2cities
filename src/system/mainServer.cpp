#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "../network/NetworkServer.h"
#include "../physics/Physics.h"
#include "../state/CarnageState.h"
#include "global.h"

#define WorldObj PhysObj

using namespace std;

void initState()
{
//	global::stateManager.changeCurrentState(CARNAGE_S);
//	CarnageState *state = (CarnageState*)global::stateManager.currentState;
}

/*
 * application kickstart.
 */
int main(int argc, char** argv) {
	initState();

	Physics physicsEngine;
    physicsEngine.initPhysics();
    physicsEngine.loadFromFile("resources/test.lvl");
    cout << "Physics initialized" << endl;

    NetworkServer network;
    network.initialize();
    cout << "Network initialized" << endl;

	while(1) {


	}

	return 0;
}
