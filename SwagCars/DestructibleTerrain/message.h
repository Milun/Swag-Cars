#ifndef MESSAGE_H
#define MESSAGE_H

#include "global.h"
#include "sprite.h"
#include "Collision.h"

using namespace std;

class Message
{
private:
	string	s;
	Point	p;
	int		i;
	bool	b;
	Sprite* spr;
	Collision* col;

public:
	inline Message() {s = ""; p = Point(); i = 0; b = false; spr = nullptr; col = nullptr;};

	// Add a key/value, while allowing the value to be any type.
	inline void addString(string _s)			{ s = _s; }
	inline void addPoint(Point _p)				{ p = _p; }
	inline void addInt(int _i)					{ i = _i; }
	inline void addBool(bool _b)				{ b = _b; }
	inline void addSprite(Sprite* _spr)			{ spr = _spr; }
	inline void addCollision(Collision* _col)	{ col = _col; }

	// Note: Do exception handling when accessing data in maps.
	inline string getString()	{return s;}
	inline Point getPoint()		{return p;}
	inline bool getBool()		{return b;}
	inline int getInt()			{return i;}
	inline Sprite* getSprite()	{return spr;}
	inline Collision* getCollision() {return col;}
};

#endif