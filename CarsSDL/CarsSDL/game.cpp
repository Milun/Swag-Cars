#include "game.h"

Game::Game()
{
	for (unsigned i = 0; i < 6; i++)
	{
		cars[i] = new Car(i*135 + 1);
	}

	spr = new Sprite("spr_charge.png");

	currentCar = -1;
}


void Game::Draw()
{
	spr->Draw(0, 380);

	double max = 0;

	if (currentCar == -1)
	{
		for (unsigned i = 0; i < 6; i++)
		{
			if (cars[i]->GetChargeMe())
			{
				if (cars[i]->GetChargeRate() > max)
				{
					max = cars[i]->GetChargeRate();
					currentCar = i;
				}
			}
		}
	}
	
	for (unsigned i = 0; i < 6; i++)
	{
		cars[i]->Draw();
	}

	if (currentCar != -1)
	{
		if (cars[currentCar]->Charge())
		{
			currentCar = -1;
		}
	}
}

Game::~Game()
{
	for (unsigned i = 0; i < 6; i++)
	{
		delete cars[i];
	}

	delete spr;
}