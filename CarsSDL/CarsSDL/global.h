#ifndef GLOBAL_H
#define GLOBAL_H

#include "SDL.h"
#include <windows.h>
#include <iostream>
#include <string>

extern SDL_Surface *screen;
extern bool gSecond;
extern long int gSeconds;
extern long int gLastMilli;

inline void CountSeconds()
{
	gSecond = false;

	SYSTEMTIME time;
	GetSystemTime(&time);
	int millis = ((time.wSecond * 1000) + time.wMilliseconds)%10000;

	if (millis < 100 && gLastMilli > 100) gLastMilli = 0;

	if (millis >= gLastMilli + 100)
	{
		gLastMilli = millis;
		gSecond = true;
		gSeconds++;
	}
}

inline std::string SecondsToTime(int s)
{
	return std::to_string(s / 60) + ":" + std::to_string(s % 60);
}

inline bool NextSecondInterval()
{
	return gSecond;
}

#endif