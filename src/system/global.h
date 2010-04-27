#include "../state/StateManager.h"
#include "../helper/camera.h"

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <map>

/*
 * All values within global namespace will be available for everyone
 */
namespace global
{

  extern StateManager stateManager; // global state manager
  
  extern int width;  // window width
  extern int height; // wondow height
  
  extern char* app_title;
  
  extern bool fullscreen;  // whether or not the window is fullscreen
  
  extern Camera camera;

  extern std::map<unsigned char, bool> keys;
  extern std::map<int, bool> special_keys;

  extern void key_down(unsigned char key, int x, int y);
  extern void key_up(unsigned char key, int x, int y);
  extern void special_key_down(int key, int x, int y);
  extern void special_key_up(int key, int x, int y);
}

#endif
