#include "game.h"
#include <iostream>

Game::Game()
{
	for (unsigned i = 0; i < 6; i++)
	{
		cars[new Car(i * 135 + 1)] = 0;
	}

	spr = new Sprite("spr_charge.png");
	text = new Text("lucon.ttf");
	textTime = new Text("lucon.ttf", 28);
}

void Game::Draw()
{
	spr->Draw(0, 380);

	text->Draw(850, 10, "Profit: $" + ThousandString(std::to_string(profit)) + ".00" );
	text->Draw(850, 40, "Total waiting time caused: " + SecondsToTime(timeWasted) + " (+" + std::to_string(noOfWaitingCars) + ") per second");

	

	textTime->Draw(850, 100, ToTime((int)gTime));
	
	// Always have an array ready of cars waiting.
	carsWaiting.clear();
	for (iterator = cars.begin(); iterator != cars.end(); iterator++)
	{
		if (iterator->first->GetChargeMe())
		{
			carsWaiting.push_back(iterator->first);
		}
	}

	ChargeCars();
	CalcWaitingTime();
}

void Game::Sort()
{
	carsEarly.clear();

	// Sort cars by waiting time.
	while (carsWaiting.size() > 0)
	{
		int maxChargeTime = -1;

		Car* tmp;
		for (unsigned i = 0; i < carsWaiting.size(); i++)
		{
			if (carsWaiting.at(i)->GetChargeTime() > maxChargeTime)
			{
				tmp = carsWaiting.at(i);
			}
		}

		carsEarly.push_back(tmp);
		carsWaiting.erase(find(carsWaiting.begin(), carsWaiting.end(), tmp));
	}

	/*int k = 0;

	do
	{
		for (unsigned i = 0; i < carsWaiting.size; i++)
		{
			if (carsWaiting.at(i)->GetChargeTime() > carsWaiting.at(i)->GetDue())
			{
				optimal = false;
				k = i;
				break;
			}
			optimal = true;
		}

		if (!optimal)
		{
			vector<Car*> temp;

			for (unsigned i = 0; i < k; i++)
			{
				Car* tmp = carsWaiting.at(i);

				if (tmp->GetChargeTime() > waiting[i + 1].ChargeTime)
				{
					waiting[i];
				}
			add tmp to late
				remove tmp from early
				sort early by due date
				sort late by due date

		}
	until optimum = true;*/
}

std::string Game::ThousandString(std::string pass)
{
	unsigned int length = strlen(pass.c_str()); 
	std::string output;

	unsigned int offset = length % 3; // Get the comma offset
	for (unsigned int i = 0; i < length; ++i) // Loop each character
	{
		// If our Index%3 == CommaOffset and this isn't first character, add a comma
		if (i % 3 == offset && i)
		{
			output += ','; // Add the comma
		}
		output += pass.at(i);
	}
	
	return output;
}

void Game::ChargeCars()
{
	if (carsWaiting.size() <= 0) return;
	if (carsEarly.size() <= 0) Sort();
	if (carsEarly.size() <= 0) return;

	currentCar = carsEarly.at(0);

	if (currentCar != nullptr && carsEarly.size() > 0)
	{
		if (!currentCar->Charge())
		{
			currentCar->StopCharge();

			carsEarly.erase(find(carsEarly.begin(), carsEarly.end(), currentCar));
			
			// Resort the list if you're done.
			//if (carsEarly.size() <= 0) Sort();
			//currentCar = carsEarly.at(0);

			currentCar = nullptr;
		}
		else
		{
			// Increase profits.
			if (NextSecondInterval())
			profit += 1;

			if (currentCar->GetChargeMe() == false)
			{
				cars[currentCar] = 0;
			}
		}
	}
}

void Game::CalcWaitingTime()
{
	noOfWaitingCars = 0;

	for (iterator = cars.begin(); iterator != cars.end(); iterator++)
	{
		iterator->first->Draw();

		// Cars that are charging do not count as "waiting" (their time is not being wasted afterall).
		if (iterator->first->GetChargeMe())
		{
			iterator->second++;
			noOfWaitingCars++;

			// Add to the overall time wasted waiting of all cars.
			if (NextSecondInterval())
			{
				timeWasted++;
				waitingSinceLast++;
			}
		}
	}

	if (NextSecondInterval() && gTime % 30 == 0)
	{
		//UpdateWaitingGraph(waitingSinceLast);
		waitingSinceLast = 0;
	}
}


/*Car* Game::NextCarToCharge()
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
}*/

/*Car* Game::NextCarToCharge()
{
	// Cars that have waited for too long are our highest priority.
	if (currentCar && currentCar->GetWaitTime() > maxWaitTime)
	{
		return currentCar;
	}

	// Only swap if the car has been charging too long (temporary).
	//if (!currentCar || currentCar->GetChargeTime() > 500)
	{

		iterator = cars.begin();

		// Go through all cars:
		for (iterator; iterator != cars.end(); iterator++)
		{
			// Ignore the current car. Don't count cars that aren't waiting.
			if ( iterator->first == currentCar ||
				!iterator->first->GetChargeMe() ) continue;

			// For starters, stop charging.
			if (currentCar) currentCar->StopCharge();

			// If we don't have a current car, give us one.
			currentCar = iterator->first;

			// If a car has exceeded the wait time, switch.
			if (iterator->first && iterator->first->GetWaitTime() >= maxWaitTime)
			{
				currentCar = iterator->first;
				return currentCar;
			}
		}

	}

	return currentCar;
}*/

Game::~Game()
{
	for (iterator; iterator != cars.end(); iterator++)
	{
		delete iterator->first;
	}

	delete spr;
	delete text;
	delete textTime;
}