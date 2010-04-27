#include "../state/StateManager.h"
#include "../helper/camera.h"
#include "global.h"

/*
 * All values within global namespace will be available for everyone
 */
 
namespace global
{

  StateManager stateManager; // global state manager
  
  int width;  // window width
  int height; // wondow height
  
  char* app_title;
  
  bool fullscreen;  // whether or not the window is fullscreen
  
  Camera camera;

  std::map<unsigned char, bool> keys;
  std::map<int, bool> special_keys;

  void key_down(unsigned char key, int x, int y) {
    keys[key] = true;
  }

  void key_up(unsigned char key, int x, int y) {
    keys[key] = false;
  }

  void special_key_down(int key, int x, int y) {
    special_keys[key] = true;
  }

  void special_key_up(int key, int x, int y) {
    special_keys[key] = false;
  }
}
