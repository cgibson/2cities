#include "UIWheelChooser.h"

const float UIWheelChooser::SPRING = 10.0;
const float UIWheelChooser::FRICTION = 0.5;

UIWheelChooser::UIWheelChooser()
	: UIControl()
{
	_numItems = 0;
	_currentItem = 0;
	_velocity = 0.0;
	_offset = 0.0;
	_icons = NULL;
	_names = NULL;
}

UIWheelChooser::~UIWheelChooser()
{
	if (_icons != NULL) free(_icons);
	if (_names != NULL) free(_names);
}

void UIWheelChooser::init(int numItems, int bucketWidth)
{
	UIControl::init(100, 100, 100, 100, 0.0, 0.0, 0.0, 1.0);

	_bucketWidth = bucketWidth;

	if (numItems <= 0)
	{
		fprintf(stderr, "Hey retard, you need at least one item in a UIWheelChooser!\n");
		exit(EXIT_FAILURE);
	}
	_numItems = numItems;

	// allocate memory for all the pointer arrays
	_icons = (UIIcon **)malloc(sizeof(UIIcon *) * _numItems);
	_names = (UILabel **)malloc(sizeof(UILabel *) * _numItems);

	// create all the items and construct their contents, but do not initalize
	// them (this falls on the overall UI because we don't yet know what items
	// the UI is going to include)
	for (int i = 0; i < _numItems; i++)
	{
		_icons[i] = new UIIcon();
		_names[i] = new UILabel();
	}
}

void UIWheelChooser::update(int ms)
{
	// update the base class
	UIControl::update(ms);

	// update this class

	// calculate the ideal offset based on the currently selected item
	float idealOffset = (_currentItem * _bucketWidth);

	// calculate the delta based on a simple spring
	float accel = (idealOffset - _offset) * SPRING;
	_velocity += accel;
	_velocity *= FRICTION;
	float delta = _velocity * (ms / 1000.0);

	// move our current offset by the delta
	_offset += delta;

	// update the items to reflect their position on the screen
	for (int i = 0; i < _numItems; i++)
	{
		// calculate the distance from the selected item
		//float idealDist = (i * _bucketWidth) - idealOffset;
		//if (idealDist < 0) idealDist *= -1;
		float actualDist = (i * _bucketWidth) - _offset;
		if (actualDist < 0) actualDist *= -1;

		// adjust the size based on the distance from the selected element
		int halfWidth = _icons[i]->naturalWidth() / 2;
		int halfHeight = _icons[i]->naturalHeight() / 2;
		if (actualDist >= _bucketWidth)
		{
			// if greater than one bucket width away, size is 1/2
			_icons[i]->size(halfWidth, halfHeight);
		}
		else if (actualDist < _bucketWidth && actualDist >= 3)
		{
			// if less than a bucket width away, but at least 3 pixels away, size is proportional
			float ratio = (_bucketWidth - actualDist) / (float)(_bucketWidth);
			//printf("ratio = %f\n", ratio);
			_icons[i]->size(halfWidth * ratio + halfWidth, halfHeight * ratio + halfHeight);
		}
		else
		{
			// if within 3 pixels, full size
			_icons[i]->size(_icons[i]->naturalWidth(), _icons[i]->naturalHeight());
		}

		// calculate the center adjustment (always center the icon in the bucket based on its size)
		float centerAdjust = _icons[i]->width() / 2;

		// place the icon
		_icons[i]->pos(i * _bucketWidth - _offset - centerAdjust, _icons[i]->naturalHeight() - _icons[i]->height());

		// now figure out the position for its name
		_names[i]->pos(_icons[i]->width() / 2, _icons[i]->height() + 10);

		// foreground alpha of the name mirrors the change in height
		_names[i]->fgclr(_names[i]->fgr(), _names[i]->fgg(), _names[i]->fgb(),
			(_icons[i]->height() - halfHeight) / (float)(_icons[i]->naturalHeight() - halfHeight));
	}

	// update our children
	updateChildren(ms);
}

void UIWheelChooser::draw()
{
	// DON'T draw the base class
	//UIControl::draw();

	// draw this class
	// nothing?

	// draw our children
	drawChildren();
}

void UIWheelChooser::checkBounds(int i)
{
	if (i < 0 || i >= _numItems)
	{
		fprintf(stderr, "ERROR: Out of bounds access in UIWheelChooser (i = %d, numItems = %d)\n", i, _numItems);
		exit(EXIT_FAILURE);
	}
}
