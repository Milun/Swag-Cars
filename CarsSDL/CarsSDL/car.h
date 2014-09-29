#ifndef CAR_H
#define CAR_H

#include "global.h"
#include "text.h"
#include "sprite.h"

class Car
{
private:
		static int count; //How many cars there are

		int id;
		int x, y;

		Text* text;
		Sprite* sprite;

		double currentCharge;
		double chargeRate;
		double maxCharge;

public:
	Car(int _x);
	~Car();

	bool Charge();

	void Draw();
};

#endif
