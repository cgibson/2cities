#ifndef _INGAMESTATE_H_
#define _INGAMESTATE_H_

class InGameState
{
    public:
       InGameState();
       virtual ~InGameState();

       virtual void initialize();
       virtual void update(int val);

    protected:
       virtual void mouse_click(int button, int state, int x, int y);
       virtual void mouse_motion(int x, int y);

};

#endif
