#ifndef _BLANKUI_H_
#define _BlANKUI_H_

#include "GameUI.h"

class BlankUI : public GameUI {
public:
    BlankUI();
    ~BlankUI();

    void init(const char *script);
    void update(int ms);
    void draw();
};

#endif
