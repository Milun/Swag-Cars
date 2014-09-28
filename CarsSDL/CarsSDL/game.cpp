#include "game.h"

Game::Game()
{
	for (unsigned i = 0; i < 6; i++)
	{
		cars[i] = new Car(i*135 + 1);
	}
}


void Game::Draw()
{
	for (unsigned i = 0; i < 6; i++)
	{
		cars[i]->Draw();
	}
}

Game::~Game()
{
	for (unsigned i = 0; i < 6; i++)
	{
		delete cars[i];
	}
}