#include "../Header Files/Truck.h"

Truck::Truck(Type type, int capacity, int maintenanceTime, int speed, int truckID)
{
	this->numOfCargos = capacity; 
	this->type = type;
	this->capacity = capacity;
	this->maintenanceTime = maintenanceTime;
	inMaintenance = false;
	this->deliveryJourneys = 0;
	this->speed = speed;
	deliveryInterval = 0;
	totalActiveTime = 0;
	this->truckID = truckID;
}

bool Truck::setInMaintenance(int deliveryJourneysBeforeMaintenance)
{
	if (deliveryJourneys >= deliveryJourneysBeforeMaintenance)
	{
		inMaintenance = true;
		return true;
	}
	if (deliveryJourneysBeforeMaintenance == 0)
		inMaintenance = false; //Maintenance is Over
	return false;
}

void Truck::setdeliveryInterval()
{
	int distanceOfFurthest = 0;
	int sumOfUnloadTimes = 0;
	Cargo* ptr = nullptr;
	for (int i = 0; i < numOfCargos; i++)
	{
		pCargo.getEntry(i, ptr);
		if (ptr->getDeliveryDistance() > distanceOfFurthest)
			distanceOfFurthest = ptr->getDeliveryDistance();
	}
	deliveryInterval = 2 * float((distanceOfFurthest / speed)) + sumOfUnloadTimes;
}

Time Truck::getComebackTime()
{
	return MoveTime; //COME BACK LATER
	//float comebackTime = (MoveTime.toInt() + deliveryInterval);
	//return comebackTime.t
}

float Truck::getTotalActiveTime()
{
	int distanceOfFurthest = 0;
	int sumOfUnloadTimes = 0;
	Cargo* ptr = nullptr;
	for (int i = 0; i < numOfCargos; i++)
	{
		pCargo.getEntry(i, ptr);
		if (ptr->getDeliveryDistance() > distanceOfFurthest)
			distanceOfFurthest = ptr->getDeliveryDistance();
	}
	totalActiveTime =  float((distanceOfFurthest / speed)) + sumOfUnloadTimes;
	return totalActiveTime;
}

float Truck::getTruckUtilizationTime(int simulationTime)
{
	//assuming all cargos are delivered
	return (numOfCargos / (capacity * deliveryJourneys) * (totalActiveTime / simulationTime)) * 100;
}

void Truck::insertInList(Cargo* &item)
{
	pCargo.insert(item->getID(), item);
	numOfCargos++;
	if(numOfCargos == capacity)
	{
		setdeliveryInterval();
	}
}

void Truck::removeFromList(Cargo* &item)
{
	pCargo.remove(item->getID()); //Assuming position in list is ID
}

Truck::~Truck()
{
}
