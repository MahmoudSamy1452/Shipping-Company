#pragma once
#include "../DEFS.h"
#include "Time.h"
#include "UI.h"
#include <string>

class Truck;

class Cargo
{
private:

	Time ready_time;
	int loading_time;
	Type cargoType;
	int ID;
	int deliveryDistance;
	int cost;
	int deliveryTime;

public:

	friend ostream& operator <<(ostream& output, Cargo*& c);

	Cargo(Type cargoType, int ID, int deliveryDistance, Time readyTime, int loadingTime, int cost);

	void setType(Type cargoType);

	void setDeliveryTime(int t);

	int getDeliveryTime();

	Type getType();

	void setDeliveryDistance(float deliveryDistance);

	void addExtraMoney(int money);

	float getDeliveryDistance();

	float getCost();

	int getWaitingTime(Time now);

	Time getReadyTime();

	int getLoadingTime();

	int getPriority();

	int getID() const;

	void Print(std::string& str);

};



