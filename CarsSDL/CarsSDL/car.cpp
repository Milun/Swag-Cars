#include "car.h"

#include <random>

Car::Car(int _x)
{
	text = new Text("lazy.ttf");
	sprite = new Sprite("spr_car.png");

	chargeMe = false;

	x = _x;
	y = 0;
	chargeCurrent = 100.0;
	chargeRate = (float)(rand() % 20 + 1)/100.0f;
	chargeUse = (float)(rand() % 20 + 1) / 100.0f;
	chargeMax = 100.0;
}

void Car::Update()
{
	// Drain battery
	if (chargeCurrent > 0.0 && !chargeMe)
	{
		chargeCurrent -= chargeUse;
		if (chargeCurrent < 0.0) chargeCurrent = 0.0;
	}
	else
	{
		if (!chargeMe)
		{
			chargeMe = true;
			y = 180;
			return;
		}
	}
}

void Car::Draw()
{
	Update();

	sprite->Draw(x, y);
	text->Draw(x+20, y+10, "Charge: " + std::to_string(((int)chargeCurrent)) + "%");
	text->Draw(x+20, y+30, "Max: " + std::to_string((int)(chargeMax)) + "kWh");
	text->Draw(x+20, y+70, "Rate: " + std::to_string((int)(chargeRate*100.0f)) + "pf");
	text->Draw(x+20, y+90, "Use: " + std::to_string((int)(chargeUse*100.0f)) + "pf");

	std::string bar = "";
	for (unsigned i = 0; i < (int)chargeCurrent; i += 4)
	{
		bar += "|";
	}

	text->Draw(x + 24, y + 120, bar);
}

bool Car::Charge()
{
	chargeCurrent += chargeRate;

	if (chargeCurrent >= chargeMax)
	{
		chargeMe = false;
		y = 0;
		chargeCurrent = chargeMax;
		return true;
	}
	else
	{
		y = 400;
		return false;
	}
}

Car::~Car()
{
	delete text;
}