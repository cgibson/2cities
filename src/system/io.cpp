
#include <map>

namespace io {

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
  
  void mouse_click(int button, int state, int x, int y) {
    
  }

  void mouse_motion(int x, int y) {

  }
}
