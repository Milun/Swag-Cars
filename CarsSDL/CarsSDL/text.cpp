#include "text.h"
#include "global.h"

Text::Text(std::string fontname)
{
	color.r = 0;
	color.g = 0;
	color.b = 0;
	font = TTF_OpenFont("lazy.ttf", 28);
}

void Text::Draw(int x, int y, std::string content)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_Surface* surface = TTF_RenderText_Solid(font, content.c_str(), color);
	SDL_BlitSurface(surface, NULL, screen, &offset);
}

Text::~Text()
{
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}