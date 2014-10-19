#include "text.h"
#include "global.h"

Text::Text(std::string fontname, int size)
{
	font = TTF_OpenFont(fontname.c_str(), size);
}

void Text::Draw(int x, int y, std::string content, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Color	color;
	color.r = r;
	color.g = g;
	color.b = b;

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