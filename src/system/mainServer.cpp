#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "gl_helper.h"
#include "io.h"
#include "../graphics/graphics.h"
#include "../state/CarnageState.h"
#include "../network/NetworkServer.h"

E_STATE beginState;

using namespace std;

void initState()
{
	  global::stateManager->changeCurrentState(beginState);
}


void initialize()
{
  global::app_title = (char*)malloc(sizeof(char) * 80);
  sprintf(global::app_title, "2Cities : The Game");

  // initialization of all other modules
  initState();

  global::network = new NetworkServer();
  global::network->initialize();
}

/*
 * application kickstart.
 */
int main(int argc, char** argv)
{

  if(argc == 2)
  {
    if(!strcmp(argv[1], "-build"))
    {
      beginState=BUILD_STATE;
    }
  }else{
    beginState=CARNAGE_STATE;
  }

  initialize();

  char *input;

  while(1) {
    cout << "Enter Command:" << endl;
    cin >> input;

  }

  return 0;
}
