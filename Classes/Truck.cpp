#include "../Header Files/Truck.h"

int Truck::currtruckID = 1;

Truck::Truck(Type truckType, int capacity, int maintenanceTime, int speed)
{
	status = Waiting;
	this->numOfCargos = 0; 
	this->truckType = truckType;
	this->capacity = capacity;
	this->maintenanceTime = maintenanceTime;
	this->deliveryJourneys = 0;
	this->speed = speed;
	sumOfUnloadTimes = 0;
	deliveryInterval = 0;
	totalActiveTime = 0;
	truckID = currtruckID;
	currtruckID++;
	isinMaintenance = false;
}

int Truck::getID() const
{
	return truckID;
}

Type Truck::getType() const
{
	return truckType;
}

int Truck::getNoOfCargos() const
{
	return numOfCargos;
}

bool Truck::getisinMaintenance() const
{
	return isinMaintenance;
}

void Truck::setdeliveryInterval()
{
	deliveryInterval = 2 * float((distanceOfFurthest / speed)) + sumOfUnloadTimes;
}

void Truck::setisinMaintenence(bool maintenance)
{
	isinMaintenance = maintenance;
}

void Truck::setMoveTime(const Time& time)
{
	MoveTime = time;
	status = Moving;
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
	MovingC.enqueue(item,item->getDeliveryDistance());
	numOfCargos++;
	if(distanceOfFurthest < item->getDeliveryDistance())
		distanceOfFurthest = item->getDeliveryDistance();
	if (numOfCargos == capacity)
	{
		setdeliveryInterval();
	}
	sumOfUnloadTimes += item->getLoadingTime();
}

void Truck::removeFromPriorityQueue(Cargo* &item)
{
	MovingC.dequeue(item);
	numOfCargos--;
	int CargoDeliveryTime = MoveTime.toInt() + (item->getDeliveryDistance())/speed + item->getLoadingTime();
	item->setDeliveryTime(CargoDeliveryTime);
}

//float Truck::calculatefinaltime(Time Clock)
//{
//	switch (status) {
//	case Moving:
//		return (Clock.toInt() + delieveryinterval) 
//	case Loading:
//		return (Clock.toInt() + sumofUnloadTimes)
//	case Maintenece:
//		return (Clock.toInt() + maintenanceTime)
// }
//}

void Truck::PrintMovingCargo() const
{
	MovingC.Print();
}

Truck::~Truck()
{
}