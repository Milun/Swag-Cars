#ifndef CAR_H
#define CAR_H

#include "global.h"
#include "text.h"
#include "sprite.h"

class Car
{
private:
		int x, y;

		Text* text;
		Sprite* sprite;

		float charge;
		float chargeMulti;

public:
	Car(int _x);
	~Car();

	void Draw();
};

#endif
