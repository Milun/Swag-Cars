#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << SDL_GetError();
		return 1;
	}

	return 0;
}