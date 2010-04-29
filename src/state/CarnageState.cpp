#include <map>
#include <GL/glut.h>

#include "CarnageState.h"
#include "PhysObj.h"

#include "../system/global.h"
#include "../system/io.h"
#include "../helper/camera.h"
#include "../helper/Vector.h"

using namespace global;

CarnageState::CarnageState() {
   initialize();
}

CarnageState::~CarnageState() {}

void CarnageState::initialize() {
   // Keyboard Callbacks
   glutKeyboardFunc(&io::key_down);
   glutKeyboardUpFunc(&io::key_up);
   glutSpecialFunc(&io::special_key_down);
   glutSpecialUpFunc(&io::special_key_up);

   // Mouse Callbacks
   glutMouseFunc(&io::mouse_click);
   glutMotionFunc(&io::mouse_motion);
   glutPassiveMotionFunc(&io::mouse_motion);
   glutSetCursor(GLUT_CURSOR_NONE);
   
   physics.initPhysics();

   // TODO // Temp DEV hard coded world
   DummyBuildingUnit dummyBlock;
   // 25 blocks in square layout hovering 1 unit above ground place. (mass = 1, size = 1)
   /*for(int i = 0; i < 100; i+=2) {
      //dummyBlock = new DummyBuildingUnit(Vector(i/10,1,i%10),Vector(0,0,0), 0.0f, 1.0f, 1.0f);
      dummyBlock = DummyBuildingUnit();
      dummyBlock.setPosition(Vector(i/10,1,i%10));
      physics.addBuildingBlock(dummyBlock);
   }*/
   /*
   for(int i = -10; i <= 10; i++)
   {
      dummyBlock = DummyBuildingUnit();
      dummyBlock.setPosition(Vector(0,i + 10,(float)i / 3.0));
      physics.addBuildingBlock(dummyBlock);
   }*/
   
   physics.loadFromFile("resources/test.lvl");
   // TODO END // TEMP DEV CODE
}

// TODO Temp inherit from InGameState
// void CarnageState::update(long milli_time) {}

// TODO Temp inherit from InGameState
// void CarnageState::updateInput(long milli_time) {}




