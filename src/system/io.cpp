#include "io.h"

namespace io
{
    std::map<unsigned char, bool> keys;
    std::map<int, bool> special_keys;
    std::map<int, int> mouse_buttons;
    int mouse_x;
    int mouse_y;
    bool captured;
    bool captured_prev;		// Used when user left/returns to window

    // DO NOT reference these variables! they are for INTERNAL USE ONLY.
    std::vector<key_func> key_down_handlers;
    std::vector<key_func> key_up_handlers;
    std::vector<mouse_func> mouse_down_handlers;
    std::vector<mouse_func> mouse_up_handlers;
    std::vector<mouse_func> mouse_wheel_handlers;

    void init()
    {
        glutKeyboardFunc(key_down);
        glutKeyboardUpFunc(key_up);
        glutSpecialFunc(special_key_down);
        glutSpecialUpFunc(special_key_up);
        glutMouseFunc(mouse_click);
        glutMotionFunc(mouse_motion);
        glutPassiveMotionFunc(mouse_motion);
        glutEntryFunc(mouse_window);
        
        // we need to initialize our mouse button states, lest
        // we be bitten by their default values!
        mouse_buttons[GLUT_LEFT_BUTTON] = GLUT_UP;
        mouse_buttons[GLUT_RIGHT_BUTTON] = GLUT_UP;
        mouse_buttons[GLUT_MIDDLE_BUTTON] = GLUT_UP;
        mouse_buttons[MOUSE_WHEEL_UP] = GLUT_UP;
        mouse_buttons[MOUSE_WHEEL_DOWN] = GLUT_UP;
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
    	captured_prev = captured = true;
        glutSetCursor(GLUT_CURSOR_NONE);
        mouse_x = 0.0;
        mouse_y = 0.0;
        glutWarpPointer(global::width >> 1, global::height >> 1);
        glutIgnoreKeyRepeat(1);
    }

    void release_mouse()
    {
    	captured_prev = captured = false;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        glutIgnoreKeyRepeat(0);
    }

    void register_key_down(key_func handler)
    {
        // make sure we haven't already registered this function
        for (int i = 0; i < (int)key_down_handlers.size(); i++)
        {
            if (key_down_handlers[i] == handler) return; // ignore it, already registered
        }

        key_down_handlers.push_back(handler);
    }
    
    void unregister_key_down(key_func handler)
    {
        // search for the function pointer
        int pos = -1;
        for (int i = 0; i < (int)key_down_handlers.size(); i++)
        {
            if (key_down_handlers[i] == handler)
            {
                pos = i;
                break;
            }
        }

        if (pos >= 0)
        {
            // found! remove it
            key_down_handlers.erase(key_down_handlers.begin() + pos);
        }
    }

    void register_key_up(key_func handler)
    {
        // make sure we haven't already registered this function
        for (int i = 0; i < (int)key_up_handlers.size(); i++)
        {
            if (key_up_handlers[i] == handler) return; // ignore it, already registered
        }

        key_up_handlers.push_back(handler);
    }

    void unregister_key_up(key_func handler)
    {
        // search for the function pointer
        int pos = -1;
        for (int i = 0; i < (int)key_up_handlers.size(); i++)
        {
            if (key_up_handlers[i] == handler)
            {
                pos = i;
                break;
            }
        }

        if (pos >= 0)
        {
            // found! remove it
            key_up_handlers.erase(key_up_handlers.begin() + pos);
        }
    }

    void register_mouse_down(mouse_func handler)
    {
        // make sure we haven't already registered this function
        for (int i = 0; i < (int)mouse_down_handlers.size(); i++)
        {
            if (mouse_down_handlers[i] == handler) return; // ignore it, already registered
        }

        mouse_down_handlers.push_back(handler);
    }

    void unregister_mouse_down(mouse_func handler)
    {
        // search for the function pointer
        int pos = -1;
        for (int i = 0; i < (int)mouse_down_handlers.size(); i++)
        {
            if (mouse_down_handlers[i] == handler)
            {
                pos = i;
                break;
            }
        }

        if (pos >= 0)
        {
            // found! remove it
            mouse_down_handlers.erase(mouse_down_handlers.begin() + pos);
        }
    }

    void register_mouse_up(mouse_func handler)
    {
        // make sure we haven't already registered this function
        for (int i = 0; i < (int)mouse_up_handlers.size(); i++)
        {
            if (mouse_up_handlers[i] == handler) return; // ignore it, already registered
        }

        mouse_up_handlers.push_back(handler);
    }

    void unregister_mouse_up(mouse_func handler)
    {
        // search for the function pointer
        int pos = -1;
        for (int i = 0; i < (int)mouse_up_handlers.size(); i++)
        {
            if (mouse_up_handlers[i] == handler)
            {
                pos = i;
                break;
            }
        }

        if (pos >= 0)
        {
            // found! remove it
            mouse_up_handlers.erase(mouse_up_handlers.begin() + pos);
        }
    }

    void register_mouse_wheel(mouse_func handler)
    {
        // make sure we haven't already registered this function
        for (int i = 0; i < (int)mouse_wheel_handlers.size(); i++)
        {
            if (mouse_wheel_handlers[i] == handler) return; // ignore it, already registered
        }

        mouse_wheel_handlers.push_back(handler);
    }

    void unregister_mouse_wheel(mouse_func handler)
    {
        // search for the function pointer
        int pos = -1;
        for (int i = 0; i < (int)mouse_wheel_handlers.size(); i++)
        {
            if (mouse_wheel_handlers[i] == handler)
            {
                pos = i;
                break;
            }
        }

        if (pos >= 0)
        {
            // found! remove it
            mouse_wheel_handlers.erase(mouse_wheel_handlers.begin() + pos);
        }
    }

    void key_down(unsigned char key, int x, int y)
    {
        keys[key] = true;

        // fire off all handlers for key down events
        for (int i = 0; i < (int)key_down_handlers.size(); i++)
        {
            key_down_handlers[i](key, false);
        }
    }

    void key_up(unsigned char key, int x, int y)
    {
        keys[key] = false;
        
        // fire off all handlers for key up events
        for (int i = 0; i < (int)key_up_handlers.size(); i++)
        {
            key_up_handlers[i](key, false);
        }
    }

    void special_key_down(int key, int x, int y)
    {
        special_keys[key] = true;

        // fire off all handlers for key down events
        for (int i = 0; i < (int)key_down_handlers.size(); i++)
        {
            key_down_handlers[i](key, true);
        }
    }

    void special_key_up(int key, int x, int y)
    {
        special_keys[key] = false;
        
        // fire off all handlers for key up events
        for (int i = 0; i < (int)key_up_handlers.size(); i++)
        {
            key_up_handlers[i](key, true);
        }
    }
  
    void mouse_click(int button, int state, int x, int y)
    {
        mouse_buttons[button] = state;
        if(!captured)
        {
            mouse_x = x;
            mouse_y = y;
        }

        if (button == MOUSE_LEFT ||
            button == MOUSE_RIGHT ||
            button == MOUSE_MIDDLE)
        {
            if (state == GLUT_DOWN)
            {
                // fire off all handlers for mouse down events
                for (int i = 0; i < (int)mouse_down_handlers.size(); i++)
                {
                    mouse_down_handlers[i](button);
                }
            }
            else if (state == GLUT_UP)
            {
                // fire off all handlers for mouse up events
                for (int i = 0; i < (int)mouse_up_handlers.size(); i++)
                {
                    mouse_up_handlers[i](button);
                }
            }
        }
        else if (button == MOUSE_WHEEL_UP ||
                 button == MOUSE_WHEEL_DOWN)
        {
            if (state == GLUT_DOWN)
            {
                // fire off all handlers for mouse wheel events
                for (int i = 0; i < (int)mouse_wheel_handlers.size(); i++)
                {
                    mouse_wheel_handlers[i](button);
                }
            }
        }
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

    void mouse_window(int state) {
      if (state == GLUT_LEFT) {
    	  captured_prev = captured;
    	  captured = false;
      }
      else
    	  captured = captured_prev;
    }

}
