#ifndef CAR_H
#define CAR_H

#include "global.h"
#include "text.h"
#include "sprite.h"

class Car
{
private:
		int x, y;

		Text* text;
		Sprite* sprite;

		double chargeCurrent;
		double chargeRate;
		double chargeMax;
		double chargeUse;

		bool chargeMe;

		void Update();
public:
	Car(int _x);
	~Car();

	inline double GetChargeCurrent() { return chargeCurrent; };
	inline double GetChargeRate() { return chargeRate; };
	inline bool GetChargeMe() { return chargeMe; };

	bool Charge();

	void Draw();
};

#endif
