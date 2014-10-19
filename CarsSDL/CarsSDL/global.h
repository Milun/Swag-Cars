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

inline std::string ToTime(int val)
{
	std::string m = std::to_string(val / 60);
	std::string s = std::to_string(val % 60);

	if (val / 60 < 10) m = "0" + m;
	if (val % 60 < 10) s = "0" + s;

	return  m + ":" + s;
}

inline void IncrementTime()
{
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