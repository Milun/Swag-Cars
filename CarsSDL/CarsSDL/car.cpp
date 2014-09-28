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
	text->Draw(x, y, "test");
	sprite->Draw(x, y);
}

Car::~Car()
{
	delete text;
}