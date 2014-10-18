#include "car.h"

#include <random>

int Car::count = 0;

Car::Car(int _x)
{
	id = count++;
	text = new Text("lazy.ttf");
	sprite = new Sprite("spr_car.png");

	chargeMe = false;

	x = _x;
	y = 0;
	yDraw = 0.0f;

	chargeRate = (float)(rand() % 20 + 6) / 100.0f;
	chargeUse = (float)(rand() % 20 + 6)  / 100.0f;
	chargeMax = 100.0;
	chargeCurrent = chargeMax;
}

void Car::Update()
{
	// Drain battery
	if (mode == 'n' && chargeCurrent > 0.0)
	{
		y = 0;
		chargeCurrent -= chargeUse;

		if (chargeCurrent <= 0.0)
		{
			chargeCurrent = 0.0;
			mode = 'w';
		}
	}
	
	if (mode == 'w')
	{
		if (chargeCurrent >= chargeMax)
		{
			mode = 'n';
		}

		y = 180;

		if (NextSecondInterval()) waitTime++;
	}

	if (mode == 'c')
	{
		chargeCurrent += chargeRate;
		chargeTime++;
		y = 400;

		if (chargeCurrent >= chargeMax)
		{
			mode = 'n';

			chargeTime = 0;
			chargeCurrent = chargeMax;
			waitTime = 0;
		}
	}

	if (yDraw < (float)(y - 1) || yDraw >(float)(y + 1))
	{
		yDraw += ((float)y - yDraw) / 10.0f;
	}
}

void Car::Draw()
{
	Update();

	int yInt = (int)yDraw;

	sprite->Draw(x, yInt);
	text->Draw(x + 20, yInt + 10, "Charge: " + std::to_string(((int)chargeCurrent)) + "%");
	text->Draw(x + 20, yInt + 30, "Max: " + std::to_string((int)(chargeMax)) + "kWh");
	text->Draw(x + 20, yInt + 50, "Wait: " + SecondsToTime(waitTime));

	text->Draw(x + 20, yInt + 90, "Rate: " + std::to_string((int)(chargeRate*100.0f)) + "pf");
	text->Draw(x + 20, yInt + 180, "Rate: " + std::to_string(mode));
	text->Draw(x + 20, yInt + 110, "Use: " + std::to_string((int)(chargeUse*100.0f)) + "pf");

	std::string bar = "";
	for (unsigned i = 0; i < (int)chargeCurrent; i += 4)
	{
		bar += "|";
	}

	text->Draw(x + 24, y + 140, bar);
}

void Car::StopCharge()
{
	mode = 'w';
}

bool Car::Charge()
{
	mode = 'c';

	return (chargeCurrent < chargeMax);
}

Car::~Car()
{
	delete text;
}