#ifndef _IO_H_
#define _IO_H_

#include <map>

namespace io{

  extern std::map<unsigned char, bool> keys;
  extern std::map<int, bool> special_keys;

  extern void key_down(unsigned char key, int x, int y);
  extern void key_up(unsigned char key, int x, int y);
  extern void special_key_down(int key, int x, int y);
  extern void special_key_up(int key, int x, int y);
};

#endif
