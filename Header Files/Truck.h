#pragma once
#include "../ADTs/LinkedList.h"
#include "Cargo.h"

class Truck
{
private:
	LinkedList<Cargo*> pCargo;
	int numOfCargos;
	Type type;
	int capacity;
	int maintenanceTime;
	bool inMaintenance;
	int deliveryJourneys;
	int speed;
	float deliveryInterval;
	float totalActiveTime;
	int truckID;
	Time MoveTime;
public:
	Truck(Type type, int capacity, int maintenanceTime, int speed, int truckID);
	void setMoveTime(Time);
	bool setInMaintenance(int deliveryJourneysBeforeMaintenance);
	void setdeliveryInterval();
	Time getComebackTime();
	float getTotalActiveTime();
	float getTruckUtilizationTime(int simulationTime);
	void insertInList(Cargo* &item);
	void removeFromList(Cargo* &item);
	~Truck();
};