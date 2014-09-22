#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <math.h>
#include <string>

#define DELETEVECTOR(name) \
	for (unsigned i = 0; i < name.size(); i++) \
		if (name.at(i)) delete name.at(i);

#define DRAWVECTOR(name) \
	for (unsigned i = 0; i < name.size(); i++) \
		if (name.at(i)) name.at(i)->draw();


extern bool gDebug;
extern bool gDelay;

extern SDL_Window *gScreen;

extern const unsigned SCREEN_WIDTH;
extern const unsigned SCREEN_HEIGHT;

struct Point
{
	int x, y;

	Point(int _x = 0, int _y = 0)
	{
		x = _x;
		y = _y;
	}

	Point operator- (const Point& other)
	{
		return Point(x - other.x, y - other.y);
	}

	Point operator+ (const Point& other)
	{
		return Point(x + other.x, y + other.y);
	}
};

extern bool gMouseLeft;
extern bool gMouseRight;
extern Point gMouse;

extern bool gKeyLeft;
extern bool gKeyRight;
extern bool gKeyUp;
extern bool gKeyDown;
extern bool gKeySpace;
extern bool gKeyA;

struct Vector {
	float vx, vy;

	Vector(float _vx = 0.0f, float _vy = 0.0f) {
		vx = _vx;
		vy = _vy;
	}

	Vector(Point p0, Point p1) {
		vx = p1.x - p0.x;
		vy = p1.y - p0.y;
	}

	float getLength() {
		return sqrt( float(vx*vx + vy*vy) );
	}

	Vector getUnit() {
		double length = getLength();

		float ux = (float)vx/float(length);
		float uy = (float)vy/float(length);

		return Vector(ux, uy);
	}

	void operator=(Point other)
	{
		vx = float(other.x);
		vy = float(other.y);
	}
};

float toRad		(float val); // Converts degrees into radians.
void drawPixel	(Point pos);
void drawLine	(Point p0, Point p1);
void drawRec	(Point p0, Point p1);
void drawCircle	(Point centre, float rad);
SDL_Color createColor (int r, int g, int b);
void setColor	(SDL_Color c);

int stringToInt(std::string s);

#endif