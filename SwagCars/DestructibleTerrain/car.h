#include "global.h"
#include "sprite.h"

class Car
{
private:
	Sprite *spr;
	
	Point pos;

public:
	Car();

	void update();
};