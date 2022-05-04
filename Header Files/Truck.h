#pragma once
#include "../ADTs/PriorityQueue.h"
#include "../ADTs/LinkedQueue.h"
#include "Cargo.h"

class Cargo;

class Truck
{
private:
	PriorityQueue<Cargo*> MovingC;
	int numOfCargos;
	Type truckType;
	int capacity;
	int maintenanceTime;
	int deliveryJourneys;
	int speed;
	float deliveryInterval;
	float totalActiveTime;
	static int currtruckID;
	int truckID;
	Time MoveTime;
	int distanceOfFurthest; //To be used in functions total active time and delivery interval  
	int sumOfUnloadTimes;
	TruckStatus status;
	Time MaxWaitingCargo;
	Time finalTime;
public:
	friend ostream& operator <<(ostream& output, Truck*& c);
	Truck(Type truckType, int capacity, int maintenanceTime, int speed);
	int getID() const;
	Type getType() const;
	void setStatus(TruckStatus);
	TruckStatus getStatus() const;
	void resetJourneys();
	int getDeliveryJourneys() const;
	void incrementJourneys();
	int getNoOfCargos() const;
	bool isFull() const;
	void PrintMovingCargo() const;
	void setMoveTime(const Time& time);
	void setdeliveryInterval();
	float getTotalActiveTime();
	float getTruckUtilizationTime(int simulationTime);
	void load(Cargo*& item, Time clock);
	void unload(Cargo*& item);
	int calculatefinaltime(Time Clock);
	int getPriority() const;
	int getMaxWaitingCargo(Time Clock) const;
	Time getFirstArrival();
	Time getfinalTime() const;
	~Truck();
};