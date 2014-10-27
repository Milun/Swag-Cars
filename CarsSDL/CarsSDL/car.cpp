#include "car.h"

#include <iostream>
#include <random>

int Car::count = 0;

Car::Car(int _x, std::string _sprite, std::string _icon)
{
	id = count++;
	text = new Text("lucon.ttf", 11);

	sprite = new Sprite(_sprite);
	spriteIcon = new Sprite(_icon);

	spriteBubble = new Sprite("spr_bubble.png");

	chargeMe = false;

	carcolour = _icon;

	x = _x;
	y = 0;
	yDraw = 0.0f;

	chargeRate = (float)(rand() % 4 + 1) * 0.1f;
	chargeUse = (float)(/*rand() %*/ 20 + 6)  / 100.0f;
	chargeMax = 100.0;
	chargeCurrent = chargeMax;
}

void Car::Update()
{
	// Drain battery
	if (mode == 'n' && chargeCurrent > 0.0)
	{
		y = -200;
		chargeCurrent -= chargeUse;

		if (chargeCurrent <= 0.0)
		{
			chargeCurrent = 0.0;
			mode = 'w';

			// Set a random time that the car wants to leave.
			// The time will always be higher than the time required to charge the car.
			timeDue = gTime + GetChargeTime() + 2 + rand() % 20;
		}
	}
	
	if (mode == 'w')
	{
		if (chargeCurrent >= chargeMax)
		{
			mode = 'n';
		}

		y = 15;

		if (NextSecondInterval()) waitTime++;
	}

	if (mode == 'c')
	{
		chargeCurrent += chargeRate;
		y = 230;

		if (chargeCurrent >= chargeMax)
		{
			mode = 'n';

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

	if (mode == 'n' && y <= -200)
	{
		spriteBubble->Draw(x, 5);
	}
	else
	{
		sprite->Draw(x, yInt);

		text->Draw(x + 5, yInt + 10, "Chrg:     " + std::to_string(((int)chargeCurrent)) + "%");
		text->Draw(x + 5, yInt + 25, "ChrgRate: " + std::to_string((int)(chargeRate*gFramesToSeconds)) + "ps");

		text->Draw(x + 5, yInt + 50, "TimeWait: " + SecondsToTime(waitTime));

		if (timeDue + GetChargeTime() >= gTime) text->Draw(x + 5, yInt + 80, "TimeToCh: " + ToTime(GetChargeTime()), 0, 150, 0);
		else									text->Draw(x + 5, yInt + 80, "TimeToCh: " + ToTime(GetChargeTime()), 150, 0, 0);

		if (timeDue >= gTime)	text->Draw(x + 5, yInt + 95, "TimeDue:  " + SecondsToTime(timeDue), 0, 225, 0);
		else					text->Draw(x + 5, yInt + 95, "TimeDue:  " + SecondsToTime(timeDue), 225, 0, 0);


		//text->Draw(x + 20, yInt + 30, "Max: " + std::to_string((int)(chargeMax)) + "kWh");
		//text->Draw(x + 20, yInt + 110, "Use: " + std::to_string((int)(chargeUse*100.0f)) + "pf");

		std::string bar = "";
		for (unsigned i = 0; i < (int)chargeCurrent; i += 10)
		{
			bar += "|";
		}

		text->Draw(x + 24, yInt + 140, bar, 0, 200, 0);
	}
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
	delete sprite;
	delete spriteIcon;
	delete spriteBubble;
	delete text;
}