#include "Effect.h"

Effect::Effect()
{
	_position = Vector(0.0, 10.0, 0.0);
	_startTime = 0;
	_endTime = 0;
	_progression = 0.0;
	_alive = true;
}

Effect::~Effect()
{
	// nothing yet
}

void Effect::init(int durationInMS)
{
	int now = global::elapsed_ms();

	_startTime = now;
	_endTime = _startTime + durationInMS;
}

void Effect::update(int ms)
{
	int now = global::elapsed_ms();

	// compute the progression of the effect (0 -> 1) and clamp it
	_progression = (float)(now - _startTime) / (_endTime - _startTime);
	_progression = (_progression < 0.0) ? 0.0 : _progression;
	_progression = (_progression > 1.0) ? 1.0 : _progression;

	// are we dead yet?
	if (now >= _endTime)
	{
		_alive = false;
	}
}

