#ifndef BUTTON_H
#define BUTTON_H

#include "global.h"
#include "sprite.h"
#include <string>

class Button
{
	private:
		Sprite* sprite;
		int width, height;

	public:
		int		x, y;

		Button();
		Button(int nx,int ny, int nw, int nh, std::string nSprite);
		~Button();
		bool ClickedOnThisFrame();
		bool Clicked();

		void Draw();
};

#endif