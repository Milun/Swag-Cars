#include "sprite.h"

Sprite::Sprite(std::string filename)
{
	surface = IMG_Load(filename.c_str());
}


void Sprite::Draw(int x, int y)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(surface, NULL, screen, &offset);
}

Sprite::~Sprite()
{
	SDL_FreeSurface(surface);
}