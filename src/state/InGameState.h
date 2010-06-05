#ifndef _INGAMESTATE_H_
#define _INGAMESTATE_H_

#include <stdlib.h>
#include <vector>

#include "../scene/WorldObject.h"
#include "../system/enum.h"
using namespace enumeration;
using namespace std;

class InGameState
{
    public:
       vector<WorldObject *> objects;

       InGameState();
       virtual ~InGameState();

       virtual void initialize();
       virtual void update(long milli_time);
       virtual void updateInput(long milli_time);
	   void setRealStateType(E_STATE newRealStateType) {realStateType = newRealStateType;}
	   E_STATE getRealStateType() {return realStateType;}

       virtual enum E_STATE stateType() { return GLOBAL_STATE; }

    protected:
		E_STATE realStateType;
		int music_delay;
       double theta;
       double phi;
       double distance;
};

#endif
