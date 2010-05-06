#ifndef _IO_H_
#define _IO_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <map>
#include "global.h"

// use these constants like GLUT_LEFT_BUTTON for the mouse wheel events
#define MOUSE_WHEEL_UP 3
#define MOUSE_WHEEL_DOWN 4

/*
 * All values within io namespace will be available for everyone
 */
namespace io
{
    // "global" variables that provide the current state of the keyboard and mouse
    // YOU SHOULD NOT WRITE TO THESE... only read from them! :)
    extern std::map<unsigned char, bool> keys;
    extern std::map<int, bool> special_keys;
    extern std::map<int, int> mouse_buttons;
    extern float mouse_x;
    extern float mouse_y;

    // initializes and binds these handlers to glut
    void init();

    // updates the input every game loop (doesn't actually poll, since we get
    // glut events, but we do need to update warping the pointer back to the
    // center of the screen)
    void update(int ms);

    // call this to "capture" the mouse (pointer becomes invisible and warps
    // back to the center, so mouse_x and mouse_y are deltas
    void capture_mouse();

    // call this to "release" the mouse (pointer is visible, can move freely,
    // and mouse_x and mouse_y are absolute window coords)
    void release_mouse();

    // reading this value is ok, but don't write to it
    // instead use the capture_mouse() and release_mouse() functions
    extern bool captured;

    // glut event handlers (don't call these)
    void key_down(unsigned char key, int x, int y);
    void key_up(unsigned char key, int x, int y);
    void special_key_down(int skey, int x, int y);
    void special_key_up(int skey, int x, int y);
    void mouse_click(int button, int state, int x, int y);
    void mouse_motion(int x, int y);
    void mouse_window(int state);
}

#endif
