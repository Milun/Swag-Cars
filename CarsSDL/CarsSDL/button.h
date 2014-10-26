#ifndef BUTTON_H
#define BUTTON_H

#include "global.h"
#include "sprite.h"

class Button
{
	private:
		Sprite* sprite;
		int		x, y, width, height;

	public:
		Button();
		Button(int nx,int ny, int nw, int nh, Sprite* nSprite);
		~Button();
		bool ClickedOnThisFrame();

		void Draw();
};

#endif