#pragma once
#include "../DEFS.h"
#include "Time.h"
#include <string>


class Cargo
{
private:

	Time ready_time;
	int loading_time;
	Type cargoType;
	int ID;
	int deliveryDistance;
	int cost;
	bool delivered;
	bool assigned;
	Time moveTime;
	int deliveryTime;

public:



	Cargo(Type cargoType, int ID, int deliveryDistance, Time readyTime, int loadingTime, int cost);

	void setType(Type cargoType);

	void setDeliveryTime(int t);

	int getDeliveryTime();
	

	Type getType();

	void setDeliveryDistance(float deliveryDistance);

	void addExtraMoney(int money);

	float getDeliveryDistance();

	void setCost(float cost);

	float getCost();

	Time getWaitingTime();

	bool isDelivered();

	bool isAssigned();

	

	void setDeliveryStatus(bool status);

	void setAssignmentStatus(bool status);

	void setReadyTime(Time t);

	void setLoadingTime(Time t);

	Time getReadyTime();

	int getLoadingTime();

	int getPriority();

	void setMoveTime(Time t);
	

	Time getMoveTime();
	

	void setID(int ID);

	int getID() const;

	void Print(std::string& str);

};


