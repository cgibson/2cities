#ifndef _IO_H_
#define _IO_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <vector>
#include <map>
#include "global.h"

// use these constants when dealing with mouse event handlers
#define MOUSE_LEFT GLUT_LEFT_BUTTON
#define MOUSE_RIGHT GLUT_RIGHT_BUTTON
#define MOUSE_MIDDLE GLUT_MIDDLE_BUTTON
#define MOUSE_WHEEL_UP 3
#define MOUSE_WHEEL_DOWN 4

/* 
 * HOW DO I REGISTER MY EVENT HANDLERS FOR INPUT CALLBACKS?
 * 
 * Just write a function (must be static if it's a member of a class) with
 * the following signature:
 *
 * Keyboard callbacks:  void handler(int key, bool special)
 * Mouse callbacks:     void handler(int button)
 *
 * Then register the callback with the io system by calling one of the five
 * registration functions:
 *
 * io::register_key_down(handler);
 * io::register_key_up(handler);
 * io::register_mouse_down(handler);
 * io::register_mouse_up(handler);
 * io::register_mouse_wheel(handler);
 *
 * (Use ClassName::handler if it's a static function of a class.)
 *
 * You can also unregister a callback from receiving any more events using
 * the unregister_* sister functions:
 *
 * io::unregister_key_down(handler);
 * ... etc. ...
 *
 * Keyboard events receive a key and a special flag. If the special flag is
 * true, then key is one of the GLUT defined special keys like GLUT_KEY_HOME.
 * See the glutSpecialFunc man page for a full list. If the special flag is
 * false, then key is an ASCII value from the keyboard.
 *
 * Keyboard callbacks (down and up) are fired ONCE and ONLY ONCE when the
 * mouse is "captured". This means key repeats by holding down a key are
 * ignored. However, when the mouse is "released" (pointer is displayed and
 * free to move about the window) then key repeats are enabled an you will
 * receive MULTIPLE CALLBACKS if you hold down a key!
 *
 * Mouse events receive a button. This can be one of the following three
 * constants defined by the io module:
 *
 * MOUSE_LEFT
 * MOUSE_MIDDLE
 * MOUSE_RIGHT
 *
 * The mouse wheel callback also receives a button, but it's interpretation
 * is mouse wheel direction. You can check which direction the mouse wheel
 * moved by using the following constants defined by the io module:
 *
 * MOUSE_WHEEL_UP
 * MOUSE_WHEEL_DOWN
 *
 * To get the coordinates of the mouse pointer, you should poll the mouse_x
 * and mouse_y variables directly. They are updated whenever the pointer moves.
 *
 * io::mouse_x
 * io::mouse_y
 *
 * Note that if the mouse is captured (not displayed, warped to the center) those
 * will only contain deltas from the center, not actual coordinates. The mouse
 * must be released for the mouse coordinates to coordinate to actual window
 * pixel coordinates.
 */
namespace io
{
    // "global" variables that provide the current state of the keyboard and mouse
    // YOU SHOULD NOT WRITE TO THESE... only read from them! :)
    extern std::map<unsigned char, bool> keys;
    extern std::map<int, bool> special_keys;
    extern std::map<int, int> mouse_buttons;
    extern int mouse_x;
    extern int mouse_y;

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

    // types to simplify the function pointer syntax
    typedef void (*key_func)(int key, bool special);
    typedef void (*mouse_func)(int button);

    // functions to register/unregister keyboard event handlers
    void register_key_down(key_func handler);
    void unregister_key_down(key_func handler);
    void register_key_up(key_func handler);
    void unregister_key_up(key_func handler);

    // function to register/unregister mouse event handlers
    void register_mouse_down(mouse_func handler);
    void unregister_mouse_down(mouse_func handler);
    void register_mouse_up(mouse_func handler);
    void unregister_mouse_up(mouse_func handler);
    void register_mouse_wheel(mouse_func handler);
    void unregister_mouse_wheel(mouse_func handler);

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
