#ifndef GLOBAL_H
#define GLOBAL_H

#include "SDL.h"
#include <windows.h>
#include <iostream>
#include <string>

extern SDL_Surface *screen;

extern long int gMillis;
extern long int gTime;
extern int gFramesToSeconds;
extern bool gPause;

//Mouse info
extern bool clickedThisFrame;
extern bool click;
extern int clickPosX, clickPosY;
//End mouse info

/*SDL_Surface *surfaceRect;*/

/*
inline void DrawSquare(int x1, int y1, int x2, int y2)
{
	SDL_Rect r;
	r.x = x1;
	r.y = y1;
	r.w = x2-x1;
	r.h = y2-y1;

	// Set render color to blue ( rect will be rendered in this color )
	SDL_SetRenderDrawColor(screen, 0, 0, 255, 255);

	// Render rect
	SDL_RenderFillRect(renderer, &r);

	// Render the rect to the screen
	SDL_RenderPresent(renderer);
}*/

inline void DrawRect(int posX, int posY, int width, int height, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect;
	rect.x = posX;
	rect.y = posY;
	rect.w = width;
	rect.h = height;
	

	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
}

inline void DrawLine(int x1, int y1, int x2, int y2)
{
	for (unsigned i = x1; i < x2; i++)
	{

	}
}

inline std::string ToTime(int val)
{
	std::string m = std::to_string(val / 60);
	std::string s = std::to_string(val % 60);

	if (val / 60 < 10) m = "0" + m;
	if (val % 60 < 10) s = "0" + s;

	return  m + ":" + s;
}

inline double GetTimeDouble()
{
	return (double)gTime + (double)gMillis / (double)gFramesToSeconds;
}

inline void IncrementTime()
{
	if (gPause) return;

	gMillis++;

	if (gMillis > gFramesToSeconds)
	{
		gTime++;
		gMillis = 0;
	}
}

inline bool NextSecondInterval()
{
	return (gMillis == 0);
}

inline std::string SecondsToTime(int s)
{
	return std::to_string(s / 60) + ":" + std::to_string(s % 60);
}

#endif