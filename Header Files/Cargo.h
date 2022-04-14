#pragma once
#include "../DEFS.h"
#include "Time.h"


class Cargo
{
private:

	Time ready_time;
	int loading_time;
	Type type;
	int ID;
	int deliveryDistance;
	int cost;
	bool delivered;
	bool assigned;

public:

	Cargo(Type type, int ID, int deliveryDistance, Time readyTime, int loadingTime, int cost);

	void setType(Type type);

	Type getType();

	void setDeliveryDistance(float deliveryDistance);

	float getDeliveryDistance();

	void setCost(float cost);

	float getCost();

	int getWaitingTime(Time t);

	bool isDelivered();

	bool isAssigned();

	void setDeliveryStatus(bool status);

	void setAssignmentStatus(bool status);

	void setReadyTime(Time t);

	void setLoadingTime(Time t);

	Time getReadyTime();

	Time getLoadingTime();

	int getPriority();

	void setID(int ID);

	int getID();

};


