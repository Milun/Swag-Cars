#ifndef CAR_H
#define CAR_H

#include "global.h"
#include "sprite.h"
#include "text.h"

class Car
{
private:
	Sprite *spr;
	Text *text;
	Point pos;

public:
	Car(int _x);
	~Car();

	void update();
};

#endif