#include "car.h"

#include <random>

Car::Car(int _x)
{
	text = new Text("lazy.ttf");
	sprite = new Sprite("spr_car.png");

	x = _x;
	y = 0;
	charge = 0.0f;
	chargeMulti = (float)(rand() % 20)/10.0f;
}

void Car::Draw()
{
	sprite->Draw(x, y);
	text->Draw(x+20, y+10, "Charge: " + std::to_string(((int)charge)) + "%");
	text->Draw(x + 20, y + 25, "Rate: " + std::to_string((int)(chargeMulti*10.0f)) + "pf");
}

Car::~Car()
{
	delete text;
}