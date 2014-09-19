#include "car.h"

Car::Car()
{
	spr = new Sprite("spr_car.png");

	pos.x = 0;
	pos.y = SCREEN_HEIGHT;
}

void Car::update()
{
	spr->draw(pos);
}