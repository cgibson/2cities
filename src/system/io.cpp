#include "io.h"

namespace io
{
    std::map<unsigned char, bool> keys;
    std::map<int, bool> special_keys;
    std::map<int, int> mouse_buttons;
    float mouse_x;
    float mouse_y;
    bool captured;

    void init()
    {
        glutKeyboardFunc(key_down);
        glutKeyboardUpFunc(key_up);
        glutSpecialFunc(special_key_down);
        glutSpecialUpFunc(special_key_up);
        glutMouseFunc(mouse_click);
        glutMotionFunc(mouse_motion);
        glutPassiveMotionFunc(mouse_motion);
        // TODO: add mouse wheel here
        
        // we need to initialize our mouse button states, lest
        // we be bitten by their default values!
        mouse_buttons[GLUT_LEFT_BUTTON] = GLUT_UP;
        mouse_buttons[GLUT_RIGHT_BUTTON] = GLUT_UP;
        mouse_buttons[GLUT_MIDDLE_BUTTON] = GLUT_UP;
    }

    void update(int ms)
    {
        if (captured)
        {
            glutWarpPointer(global::width >> 1, global::height >> 1);
        }
    }

    void capture_mouse()
    {
        captured = true;
        glutSetCursor(GLUT_CURSOR_NONE);
        mouse_x = 0.0;
        mouse_y = 0.0;
        glutWarpPointer(global::width >> 1, global::height >> 1);
    }

    void release_mouse()
    {
        captured = false;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }

    void key_down(unsigned char key, int x, int y)
    {
        keys[key] = true;
    }

    void key_up(unsigned char key, int x, int y)
    {
        keys[key] = false;
    }

    void special_key_down(int key, int x, int y)
    {
        special_keys[key] = true;
    }

    void special_key_up(int key, int x, int y)
    {
        special_keys[key] = false;
    }
  
    void mouse_click(int button, int state, int x, int y)
    {
        mouse_buttons[button] = state;
    }

    void mouse_motion(int x, int y)
    {
        if (captured)
        {
            mouse_x = x - (global::width >> 1);
            mouse_y = y - (global::height >> 1);
        }
        else
        {
            mouse_x = x;
            mouse_y = y;
        }
    }
}
