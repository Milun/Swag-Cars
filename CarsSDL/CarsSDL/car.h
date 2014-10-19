#ifndef CAR_H
#define CAR_H

#include "global.h"
#include "text.h"
#include "sprite.h"

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
		Sprite* spriteBubble;

		double chargeCurrent;
		double chargeRate;
		double chargeMax;
		double chargeUse;

		bool chargeMe;

		void Update();
public:
	static int count; //How many cars there are

	Car(int _x);
	~Car();

	inline double GetChargeCurrent() { return chargeCurrent; };
	inline double GetChargeRate() { return chargeRate; };

	inline bool GetChargeMe() { return (mode == 'w'); };
	inline double ChargeLeft() { return chargeMax - chargeCurrent; }

	inline int GetWaitTime() { return waitTime; }

	inline int GetChargeTime() { return ((chargeMax - chargeCurrent) / chargeRate) / gFramesToSeconds; }
	inline int GetDue() { return timeDue; };

	bool Charge();
	void StopCharge();

	void Draw();
};

#endif
