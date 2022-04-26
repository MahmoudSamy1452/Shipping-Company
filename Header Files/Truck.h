#pragma once
#include "../ADTs/PriorityQueue.h"
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
public:

	friend ostream& operator <<(ostream& output, Truck*& c);
	Truck(Type truckType, int capacity, int maintenanceTime, int speed);
	int getID() const;
	Type getType() const;
	int getNoOfCargos() const;
	bool getisinMaintenance() const;
	void PrintMovingCargo() const;
	void setMoveTime(const Time& time);
	void setdeliveryInterval();
	void setisinMaintenence(bool maintenance);
	float getTotalActiveTime();
	float getTruckUtilizationTime(int simulationTime);
	void insertInPriorityQueue(Cargo* &item);
	void removeFromPriorityQueue(Cargo* &item);
	float calculatefinaltime(Time Clock);
	~Truck();
};