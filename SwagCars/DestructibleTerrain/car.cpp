#include "car.h"

Car::Car(int _x)
{
	spr = new Sprite("spr_car.png");
	text = new Text();

	pos.x = _x;
	pos.y = SCREEN_HEIGHT - 140;
}

void Car::update()
{
	spr->draw(pos);
	text->Update(Point(50, 50));
}

Car::~Car()
{
	delete text;
}
