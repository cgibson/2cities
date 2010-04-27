#ifndef _INGAMESTATE_H_
#define _INGAMESTATE_H_

class InGameState
{
    public:
       InGameState();
       virtual ~InGameState();

       virtual void initialize();
       virtual void update();

    protected:
/*
       virtual void key_down(unsigned char key, int x, int y);
       virtual void key_up(unsigned char key, int x, int y);
       virtual void special_key_down(int key, int x, int y);
       virtual void special_key_up(int key, int x, int y);
*/
       virtual void mouse_click(int button, int state, int x, int y);
       virtual void mouse_motion(int x, int y);

};

#endif
