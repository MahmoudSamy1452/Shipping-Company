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
	bool isinMaintenance;
	TruckStatus status;
	Time MaxWaitingCargo;
public:
	friend ostream& operator <<(ostream& output, Truck*& c);
	Truck(Type truckType, int capacity, int maintenanceTime, int speed);
	int getID() const;
	Type getType() const;
	int getNoOfCargos() const;
	bool isFull() const;
	bool getisinMaintenance() const;
	void PrintMovingCargo() const;
	void setMoveTime(const Time& time);
	void setdeliveryInterval();
	void setisinMaintenence(bool maintenance);
	float getTotalActiveTime();
	float getTruckUtilizationTime(int simulationTime);
	void load(Cargo*& item, Time clock);
	void unload(Cargo*& item, Time clock);
	float calculatefinaltime(Time Clock);
	int getPriority() const;
	int getMaxWaitingCargo(Time Clock) const;
	~Truck();
};