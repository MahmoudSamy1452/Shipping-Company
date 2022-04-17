#include "../Header Files/Truck.h"

Truck::Truck(Type truckType, int capacity, int maintenanceTime, int speed, int truckID)
{
	this->numOfCargos = capacity; 
	this->truckType = truckType;
	this->capacity = capacity;
	this->maintenanceTime = maintenanceTime;
	this->deliveryJourneys = 0;
	this->speed = speed;
	deliveryInterval = 0;
	totalActiveTime = 0;
	this->truckID = truckID;
}

void Truck::setdeliveryInterval()
{
	deliveryInterval = 2 * float((distanceOfFurthest / speed)) + sumOfUnloadTimes;
}

void Truck::setMoveTime(const Time& time)
{
	MoveTime = time;
}

Time Truck::getComebackTime()
{
	return Time(MoveTime.toInt() + deliveryInterval); //COME BACK LATER
	//float comebackTime = (MoveTime.toInt() + deliveryInterval);
	//return comebackTime.t
}

float Truck::getTotalActiveTime()
{
	totalActiveTime =  float((distanceOfFurthest / speed)) + sumOfUnloadTimes;
	return totalActiveTime;
}

float Truck::getTruckUtilizationTime(int simulationTime)
{
	//assuming all cargos are delivered
	return (numOfCargos / (capacity * deliveryJourneys) * (totalActiveTime / simulationTime)) * 100;
}

void Truck::insertInPriorityQueue(Cargo* &item)
{
	pCargo.enqueue(item,item->getDeliveryDistance());
	numOfCargos++;
}

void Truck::removeFromPriorityQueue(Cargo* &item)
{
	pCargo.dequeue(item);
	if (pCargo.isEmpty())
	{
		distanceOfFurthest = item->getDeliveryDistance();
		setdeliveryInterval();
	}
	sumOfUnloadTimes += item->getLoadingTime();
}

Truck::~Truck()
{
}