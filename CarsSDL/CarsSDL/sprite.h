#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>

#include "global.h"

class Sprite
{
private:
	SDL_Surface* surface;

public:
	Sprite(std::string filename);
	~Sprite();

	void Draw(int x, int y);
};

#endif