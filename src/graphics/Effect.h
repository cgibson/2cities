#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "../system/global.h"

class Effect {
public:
    Effect();
    virtual ~Effect();

    virtual void init(int durationInMS);
    virtual void update(int ms);
    virtual void draw() = 0;

    // property accessors
	Vector position() const { return _position; }
	void position(Vector position) { _position = position; }
	bool alive() const { return _alive; }
	void alive(bool alive) { _alive = alive; }

protected:
	Vector _position;
	int _startTime;
	int _endTime;
	float _progression;
	bool _alive;
};

#endif
