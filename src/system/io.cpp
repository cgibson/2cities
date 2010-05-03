#include <map>
#include "global.h"

namespace io {

  std::map<unsigned char, bool> keys;
  std::map<int, bool> special_keys;
  std::map<int, int> mouse_buttons;
  float mouse_x;
  float mouse_y;
  int warp_mouse;

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
  
  void mouse_click(int button, int state, int x, int y) {
     mouse_buttons[button] = state;
  }

  void mouse_motion(int x, int y) {
	x -= (global::width >> 1);
	y -= (global::height >> 1);

    mouse_x += x;
    mouse_y += y;
  }

  void mouse_window(int state) {
    if (state == GLUT_LEFT)
    	warp_mouse = false;
    else
    	warp_mouse = true;
  }
}
