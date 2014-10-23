#include "game.h"
#include <iostream>

Game::Game()
{
	spr = new Sprite("spr_charge.png");
	text = new Text("lucon.ttf");
	textTime = new Text("lucon.ttf", 28);

	color[0].r = 255; color[0].g = 0; color[0].b = 0;
	color[1].r = 0; color[1].g = 255; color[1].b = 0;
	color[2].r = 0; color[2].g = 0; color[2].b = 255;
	color[3].r = 255; color[3].g = 150; color[3].b = 0;
	color[4].r = 255; color[4].g = 0; color[4].b = 255;
	color[5].r = 0; color[5].g = 255; color[5].b = 255;
	color[6].r = 0; color[6].g = 0; color[6].b = 0;
	color[7].r = 100; color[7].g = 0; color[7].b = 255;
	color[8].r = 0; color[8].g = 255; color[8].b = 100;
	color[9].r = 255; color[9].g = 0; color[9].b = 100;

	for (unsigned i = 0; i < 6; i++)
	{
		cars[new Car(i * 135 + 1, color[i])] = 0;
	}
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

	DrawSchedule();
}

void Game::DrawSchedule()
{
	for (unsigned i = 0; i < carsByDue.size(); i++)
	{
		textTime->Draw(40 + 70 * i, 710, "X", carsByDue.at(i)->GetColor().r, carsByDue.at(i)->GetColor().g, carsByDue.at(i)->GetColor().b);
		textTime->Draw(42 + 70 * i, 710, "X", carsByDue.at(i)->GetColor().r, carsByDue.at(i)->GetColor().g, carsByDue.at(i)->GetColor().b);
		textTime->Draw(42 + 70 * i, 709, "X", carsByDue.at(i)->GetColor().r, carsByDue.at(i)->GetColor().g, carsByDue.at(i)->GetColor().b);
	}

	for (unsigned i = 0; i < carsFinal.size(); i++)
	{
		textTime->Draw(40 + 70 * i, 750, "X", carsFinal.at(i)->GetColor().r, carsFinal.at(i)->GetColor().g, carsFinal.at(i)->GetColor().b);
		textTime->Draw(42 + 70 * i, 750, "X", carsFinal.at(i)->GetColor().r, carsFinal.at(i)->GetColor().g, carsFinal.at(i)->GetColor().b);
		textTime->Draw(42 + 70 * i, 749, "X", carsFinal.at(i)->GetColor().r, carsFinal.at(i)->GetColor().g, carsFinal.at(i)->GetColor().b);
	}
}

void Game::Sort()
{
	// Clear the lists for a new sort.
	carsByDue.clear();
	carsLate.clear();
	carsFinal.clear();

	// Sort cars by the time they are due.
	while (carsWaiting.size() > 0)
	{
		long int minDueTime = 9999999;

		Car* tmp;
		for (unsigned i = 0; i < carsWaiting.size(); i++)
		{
			if (carsWaiting.at(i)->GetDueTime() < minDueTime)
			{
				minDueTime = carsWaiting.at(i)->GetDueTime();
				tmp = carsWaiting.at(i);
			}
		}

		carsByDue.push_back(tmp);
		carsWaiting.erase(find(carsWaiting.begin(), carsWaiting.end(), tmp));
	}

	// Now, go through the sorted cars and find the first car that will be late.
	bool optimal = false;
	while (!optimal)
	{
		long int addTime = 0;

		// Find the first car which will be late.
		for (unsigned i = 0; i < carsByDue.size(); i++)
		{
			addTime += carsByDue.at(i)->GetChargeTime();

			// If this car will be late...
			if (addTime > carsByDue.at(i)->GetDueTime())
			{
				long int maxChargeTime = -1;
				Car* temp = nullptr;

				for (unsigned j = 0; j < i; j++)
				{
					if (carsByDue.at(j)->GetChargeTime() > maxChargeTime)
					{
						temp = carsByDue.at(j);
						maxChargeTime = temp->GetChargeTime();
					}
				}

				// Remove the longest task and put it into another list.
				if (temp)
				{
					carsLate.push_back(temp);
					carsByDue.erase(find(carsByDue.begin(), carsByDue.end(), temp));
				}

				break;
			}

			optimal = true;
		}
	}

	for (unsigned i = 0; i < carsByDue.size(); i++)
	{
		carsFinal.push_back(carsByDue.at(i));
	}
	for (unsigned i = 0; i < carsLate.size(); i++)
	{
		carsFinal.push_back(carsLate.at(i));
	}
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
	if (carsFinal.size() <= 0) Sort();
	if (carsFinal.size() <= 0) return;

	currentCar = carsFinal.at(0);

	if (currentCar != nullptr && carsFinal.size() > 0)
	{
		if (!currentCar->Charge())
		{
			currentCar->StopCharge();

			carsFinal.erase(find(carsFinal.begin(), carsFinal.end(), currentCar));
			
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