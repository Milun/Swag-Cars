#ifndef CAR_H
#define CAR_H

#include <string>

#include "global.h"
#include "text.h"
#include "sprite.h"
#include <math.h>

class Car
{
private:
		int id;
		int x, y;
		float yDraw;

		int timeDue = 0;

		char mode = 'n';

		long int waitTime = 0;

		Text* text;
		Sprite* sprite;
		Sprite* spriteIcon;
		Sprite* spriteBubble;

		double chargeCurrent;
		double chargeRate;
		double chargeMax;
		double chargeUse;

		bool chargeMe;

		void Update();
public:
	static int count; //How many cars there are

	Car(int _x, std::string _sprite, std::string _icon);
	~Car();

	inline double GetChargeCurrent() { return chargeCurrent; };
	inline double GetChargeRate() { return chargeRate; };

	inline bool GetChargeMe() { return (mode == 'w'); };
	inline double ChargeLeft() { return chargeMax - chargeCurrent; }

	inline int GetWaitTime() { return waitTime; }

	inline float GetChargeTime() { return ((((float)chargeMax - ((float)chargeMax)) / ((float)chargeRate)) / (float)gFramesToSeconds); }
	inline float GetMaxChargeTime() { return ((((float)chargeMax) / ((float)chargeRate)) / (float)gFramesToSeconds); }

	inline int GetDueTime() { return timeDue; };

	inline Sprite* GetIcon() { return spriteIcon; };

	bool Charge();
	void StopCharge();

	void Draw();
};

#endif
