#include "car.h"

#include <iostream>
#include <random>

int Car::count = 0;

Car::Car(int _x, std::string _sprite, std::string _icon, SDL_Color _color)
{
	id = count++;
	text = new Text("lucon.ttf", 11);

	sprite = new Sprite(_sprite);
	spriteIcon = new Sprite(_icon);

	spriteBubble = new Sprite("spr_bubble.bmp");
	btnBubble = new Button(_x, 5, 122, 135, "spr_bubble.bmp");
	btnReturn = new Button(_x + 21, 5, 80, 48, "spr_return.bmp");

	btnL1 = new Button(_x + 66, 5, 16, 16, "spr_left.bmp");
	btnL2 = new Button(_x + 66, 5, 16, 16, "spr_left.bmp");
	btnR1 = new Button(_x + 110, 5, 16, 16, "spr_right.bmp");
	btnR2 = new Button(_x + 110, 5, 16, 16, "spr_right.bmp");

	chargeMe = false;

	color = _color;

	carcolour = _icon;

	x = _x;
	y = -200;
	yDraw = 0.0f;

	chargeRate = (float)(rand() % 4 + 1) * 0.1f;
	chargeUse = (float)(/*rand() %*/ 20 + 6)  / 100.0f;
	chargeMax = 100.0;
	chargeCurrent = chargeMax;

	addTime = GetChargeTime() + 2 + rand() % 20;
}

void Car::Update()
{
	// Drain battery
	if (mode == 'n' && chargeCurrent > 0.0)
	{
		y = -200;

		if (!gPause && active)
		{
			chargeCurrent -= chargeUse;

			if (chargeCurrent <= 0.0)
			{
				chargeCurrent = 0.0;
				mode = 'w';

				// Set a random time that the car wants to leave.
				// The time will always be higher than the time required to charge the car.
				timeDue = gTime + addTime;
			}
		}
	}
	
	if (mode == 'w')
	{
		y = 15;

		if (!gPause)
		{
			if (chargeCurrent >= chargeMax)
			{
				mode = 'n';
			}
			

			if (NextSecondInterval()) waitTime++;
		}
	}

	if (mode == 'c')
	{
		
		y = 230;

		if (!gPause)
		{
			chargeCurrent += chargeRate;
			if (chargeCurrent >= chargeMax)
			{
				mode = 'n';

				chargeCurrent = chargeMax;
				waitTime = 0;
			}
		}
	}
}

void Car::Draw()
{
	Update();

	if (yDraw < (float)(y - 1) || yDraw >(float)(y + 1))
	{
		yDraw += ((float)y - yDraw) / 10.0f;
	}

	int yInt = (int)yDraw;

	if (mode == 'n' && y <= -200)
	{
		DrawRect(x, y, 122, 135, color.r, color.g, color.b);

		if (btnBubble->ClickedOnThisFrame() && gPause)
		{
			mode = 'w';
			chargeCurrent = 0.0;
			timeDue = gTime + addTime;
			active = false;
		}

		btnBubble->Draw();
	}
	else
	{
		active = true;

		//sprite->Draw(x, yInt);
		DrawRect(x, yInt, 122, 192, color.r, color.g, color.b);


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

		if (gPause)
		{
			btnReturn->y = yInt + 160;
			btnReturn->Draw();
		}

		if (btnReturn->ClickedOnThisFrame() && gPause)
		{
			Reset();
		}

		if (gPause)
		{
			btnL1->y = yInt + 75;
			btnR1->y = yInt + 75;
			btnL2->y = yInt + 92;
			btnR2->y = yInt + 92;

			if (btnL1->Clicked())
			{
				if (GetChargeTime() > 0.01)
				{
					chargeRate += 0.0005;
				}
			}
			if (btnR1->Clicked())
			{
				if (chargeRate > 0.005)
				{
					chargeRate -= 0.0005;

					if (timeDue < gTime + GetMaxChargeTime() + 1)
					{
						addTime = GetMaxChargeTime() + 1;
						timeDue = gTime + addTime;
					}
				}
			}

			if (btnR2->Clicked())
			{
				addTime += 0.1;
				timeDue = gTime + addTime;
			}
			if (btnL2->Clicked())
			{
				if (timeDue > gTime + GetMaxChargeTime() + 1)
				{
					addTime -= 0.1;
					timeDue = gTime + addTime;
				}
			}

			btnL1->Draw();
			btnR1->Draw();
			btnL2->Draw();
			btnR2->Draw();
		}
	}
}

void Car::StopCharge()
{
	mode = 'w';
}

void Car::Reset()
{
	mode = 'n';
	y = -200;
	chargeCurrent = chargeMax;
	active = false;
	//addTime = GetChargeTime() + 2 + rand() % 20;
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
	delete btnBubble;
	delete btnReturn;
	delete btnL1;
	delete btnL2;
	delete btnR1;
	delete btnR2;
}