#include"button.h"

Button::Button()
{
	x = 0;
	y = 0;
	sprite = nullptr;
}

Button::Button(int nx, int ny, int nw, int nh, Sprite* nsprite)
{
	x = nx; y = ny; width = nw;  height = nh;  sprite = nsprite;
}

bool Button::ClickedOnThisFrame()
{
	if (clickedThisFrame)
	{
		int clickX = clickPosX;
		int clickY = clickPosY;

		if (clickX < x + width && clickX > x)
		{
			if (clickY < y + height && clickY > y)
			{
				return true;
			}
		}
	}

	return false;
}

void Button::Draw()
{
	sprite->Draw(x, y);
}