#include "game.h"

Game::Game()
{
	for (unsigned i = 0; i < 6; i++)
	{
		cars[new Car(i * 135 + 1)] = 0;
	}

	spr = new Sprite("spr_charge.png");

	currentCar = -1;
}

void Game::Draw()
{
	spr->Draw(0, 380);

	double max = 0;

	Car* nextCar = NextCarToCharge();

	if (nextCar != nullptr)
	{
		nextCar->Charge();

		if (nextCar->GetChargeMe() == false)
		{
			cars[nextCar] = 0;
		}
	}
	
	for (iterator = cars.begin(); iterator != cars.end(); iterator++)
	{
		(iterator->first)->Draw();

		if ((iterator->first)->GetChargeMe())
		{
			iterator->second++;
		}
	}
}

Car* Game::NextCarToCharge()
{
	Car* car = nullptr;

	iterator = cars.begin();
	car = iterator->first;
	for (iterator; iterator != cars.end(); iterator++)
	{
		if (iterator->second > cars[car])
		{
			car = iterator->first;
		}
	}

	return car;
}

Game::~Game()
{
	for (iterator; iterator != cars.end(); iterator++)
	{
		delete iterator->first;
	}

	delete spr;
}