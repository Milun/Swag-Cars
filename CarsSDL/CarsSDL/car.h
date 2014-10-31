#ifndef CAR_H
#define CAR_H

#include <string>

#include "global.h"
#include "text.h"
#include "sprite.h"
#include "button.h"
#include <math.h>

class Car
{
private:
		int id;
		int x, y;
		float yDraw;

		double timeDue = 0.0;
		double addTime = 0.0;

		SDL_Color color;

		char mode = 'n';

		double waitTime = 0;

		Text* text;
		Sprite* sprite;
		Sprite* spriteIcon;
		Sprite* spriteBubble;

		Button* btnBubble;
		Button* btnReturn;

		Button* btnL1;
		Button* btnL2;
		Button* btnR1;
		Button* btnR2;

		double chargeCurrent;
		double chargeRate;
		double chargeMax;
		double chargeUse;

		

		bool chargeMe;

		bool active = false;

		void Update();
		
public:
	std::string carcolour;

	void Reset();

	static int count; //How many cars there are

	Car(int _x, std::string _sprite, std::string _icon, SDL_Color _color);
	~Car();

	inline double GetChargeCurrent() { return chargeCurrent; };
	inline double GetChargeRate() { return chargeRate; };

	inline bool GetChargeMe() { return (mode == 'w'); };
	inline double ChargeLeft() { return chargeMax - chargeCurrent; }

	inline double GetWaitTime() { return waitTime; }

	inline double GetChargeTime() { return ((chargeMax - chargeCurrent) / chargeRate) / gFramesToSeconds; }
	inline double GetMaxChargeTime() { return (chargeMax / chargeRate) / gFramesToSeconds; }

	inline double GetDueTime() { return timeDue; };

	inline Sprite* GetIcon() { return spriteIcon; };
	inline SDL_Color GetColor() { return color; };

	bool Charge();
	void StopCharge();

	void Draw();
};

#endif
