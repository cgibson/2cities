#ifndef _UIWHEELCHOOSER_H_
#define _UIWHEELCHOOSER_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
    #include <winGL/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../system/global.h"
#include "UIControl.h"
#include "UILabel.h"
#include "UIIcon.h"
#include "UIProgress.h"

class UIWheelChooser : public UIControl {
public:
	UIWheelChooser();
	~UIWheelChooser();

	void init(int numItems, int bucketWidth);
	void update(int ms);
	void draw();

	// movement controls
	void nextItem() { _currentItem++; if (_currentItem >= _numItems) { _currentItem = _numItems - 1; } else { global::soundManager->playUIKey(); } }
	void prevItem() { _currentItem--; if(_currentItem < 0) { _currentItem = 0; } else { global::soundManager->playUIKey(); } }
	void selectItem(int item) { checkBounds(item); _currentItem = item; }

	// property accessors
	int currentItem() const { return _currentItem; }
	UIIcon *icon(int item) { checkBounds(item); return _icons[item]; }
	UILabel *name(int item) { checkBounds(item); return _names[item]; }

private:
	int _bucketWidth;
	int _numItems;
	int _currentItem;
	float _velocity;
	float _offset;

	UIIcon **_icons;
	UILabel **_names;

	void checkBounds(int i);

	static const float SPRING;
	static const float FRICTION;
};

#endif
