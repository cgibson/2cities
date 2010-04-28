#ifndef _IO_H_
#define _IO_H_

#include <map>

/*
 * All values within io namespace will be available for everyone
 */
namespace io
{
  extern std::map<unsigned char, bool> keys;
  extern std::map<int, bool> special_keys;

  extern void key_down(unsigned char key, int x, int y);
  extern void key_up  (unsigned char key, int x, int y);

  extern void special_key_down(int skey, int x, int y);
  extern void special_key_up  (int skey, int x, int y);

  extern void mouse_click(int button, int state, int x, int y);
  extern void mouse_motion(int x, int y);
}

#endif
