#pragma once
#include "../ADTs/PriorityQueue.h"
#include "Cargo.h"

class Truck
{
private:
	PriorityQueue<Cargo*> pCargo;
	int numOfCargos;
	Type truckType;
	int capacity;
	int maintenanceTime;
	int deliveryJourneys;
	int speed;
	float deliveryInterval;
	float totalActiveTime;
	int truckID;
	Time MoveTime;
	int distanceOfFurthest; //To be used in functions total active time and delivery interval  
	int sumOfUnloadTimes;
public:
	Truck(Type truckType, int capacity, int maintenanceTime, int speed, int truckID);
	void setMoveTime(const Time& time);
	void setdeliveryInterval();
	Time getComebackTime();
	float getTotalActiveTime();
	float getTruckUtilizationTime(int simulationTime);
	void insertInPriorityQueue(Cargo* &item);
	void removeFromPriorityQueue(Cargo* &item);
	~Truck();
};