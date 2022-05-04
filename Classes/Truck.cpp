#include "../Header Files/Truck.h"
#include <cmath>

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
}

int Truck::getID() const
{
	return truckID;
}

Type Truck::getType() const
{
	return truckType;
}

void Truck::setStatus(TruckStatus status)
{
	this->status = status;
}

TruckStatus Truck::getStatus() const
{
	return status;
}

void Truck::resetJourneys()
{
	deliveryJourneys = 0;
}

void Truck::incrementJourneys()
{
	deliveryJourneys++;
}

int Truck::getDeliveryJourneys() const
{
	return deliveryJourneys;
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

Time Truck::getFirstArrival()
{
	Cargo* cargo;
	if (MovingC.peek(cargo)) 
	{
		float dist = cargo->getDeliveryDistance();
		int load = cargo->getLoadingTime();
		return Time(MoveTime.toInt() + ceil(dist / speed) + load);
	}
	return Time();
}

void Truck::setdeliveryInterval()
{
	deliveryInterval = ceil(2 *(float)distanceOfFurthest / speed + sumOfUnloadTimes);
}

void Truck::setMoveTime(const Time& time)
{
	MoveTime = time;
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
	sumOfUnloadTimes += item->getLoadingTime();
	if(numOfCargos == 1)
		MaxWaitingCargo = clock;
}

void Truck::unload(Cargo* &item)
{
	MovingC.dequeue(item);
	item->setLoadedTime(Time());
	sumOfUnloadTimes -= item->getLoadingTime();
	numOfCargos--;
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

int Truck::calculatefinaltime(Time Clock)
{
	switch (status) {
	case Moving:
		finalTime = Time(Clock.toInt() + deliveryInterval);
		break;
	case Loading:
		finalTime = Time(Clock.toInt() + sumOfUnloadTimes);
		break;
	case Maintenance:
		finalTime = Time(Clock.toInt() + maintenanceTime);
		break;
	}
	return finalTime.toInt();
}

void Truck::PrintMovingCargo() const
{
	MovingC.Print();
}

Time Truck::getfinalTime() const
{
	return finalTime;
}

Truck::~Truck()
{
}