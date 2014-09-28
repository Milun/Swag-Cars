#include "car.h"

Car::Car(int _x)
{
	text = new Text("lazy.ttf");
	sprite = new Sprite("spr_car.png");

	x = _x;
	y = 0;
}

void Car::Draw()
{
	text->Draw(x, y, "Current Charge: ");
	sprite->Draw(x, y);
}

bool Car::Charge()
{
	currentCharge += chargeRate;

	if (currentCharge >= maxCharge)
	{
		currentCharge = maxCharge;
		return true;
	}
	else
	{
		return false;
	}
}

Car::~Car()
{
	delete text;
}