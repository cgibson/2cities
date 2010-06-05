/**
 * Represents the Menu State in each Two Cities game.
 */

#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include <stdlib.h>
#include "InGameState.h"
#include "../system/enum.h"

using namespace std;

class MenuState: public InGameState
{
    public:
        MenuState();
        virtual ~MenuState();
		void update(long milli_time);
        void updateInput(long milli_time);

        void initialize();
        
    protected:
		static const int MUSIC_DELAY;
};

#endif
