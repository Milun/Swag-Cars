#include "game.h"

Game::Game()
{
	for (unsigned i = 0; i < 6; i++)
	{
		cars[i] = new Car(i*120);
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