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

bool Truck::isFull() const
{
	return numOfCargos == capacity;
}

int Truck::getPriority() const
{
	return (capacity * 2 + speed * 3) * -1;
}

int Truck::getMaxWaitingCargo(Time Clock) const
{
	return Clock.toInt() - MaxWaitingCargo.toInt();
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
	totalActiveTime =  float(distanceOfFurthest / speed) + sumOfUnloadTimes;
	return totalActiveTime;
}

float Truck::getTruckUtilizationTime(int simulationTime)
{
	//assuming all cargos are delivered
	return (numOfCargos / (capacity * deliveryJourneys) * (totalActiveTime / simulationTime)) * 100;
}

void Truck::load(Cargo* &item, Time clock)
{
	MovingC.enqueue(item,item->getDeliveryDistance());
	item->setLoadedTime(clock);
	numOfCargos++;
	if(distanceOfFurthest < item->getDeliveryDistance())
		distanceOfFurthest = item->getDeliveryDistance();
	if (numOfCargos == capacity)
		setdeliveryInterval();
	sumOfUnloadTimes += item->getLoadingTime();
	if(numOfCargos == 1)
		MaxWaitingCargo = clock;
}

void Truck::unload(Cargo* &item, Time clock)
{
	MovingC.dequeue(item);
	numOfCargos--;
	int CargoDeliveryTime = MoveTime.toInt() + (item->getDeliveryDistance())/speed + item->getLoadingTime();
	item->setDeliveryTime(CargoDeliveryTime);
	if (!MoveTime.isValid() && !MovingC.isEmpty())
	{
		Cargo* tempC;
		LinkedQueue<Cargo*> tempQ;
		MovingC.dequeue(tempC);
		MaxWaitingCargo = tempC->getLoadedTime();
		while(MovingC.dequeue(tempC))
		{
			if (MaxWaitingCargo.toInt() > tempC->getLoadedTime().toInt())
				MaxWaitingCargo = tempC->getLoadedTime();
			tempQ.enqueue(tempC);
		}
		while(tempQ.dequeue(tempC))
		{
			MovingC.enqueue(tempC, tempC->getDeliveryDistance());
		}
	}
}

float Truck::calculatefinaltime(Time Clock)
{
	switch (status) {
	case Moving:
		return (Clock.toInt() + deliveryInterval);
	case Loading:
		return (Clock.toInt() + sumOfUnloadTimes);
	case Maintenance:
		return (Clock.toInt() + maintenanceTime);
	}
}

void Truck::PrintMovingCargo() const
{
	MovingC.Print();
}

Truck::~Truck()
{
}