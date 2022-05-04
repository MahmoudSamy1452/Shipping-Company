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
	countN = 0;
	countS = 0;
	countV = 0;
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
	switch(item->getType())
	{
	case Normal:
		countN++;
		break;
	case VIP:
		countV++;
		break;
	case Special:
		countS++;
		break;
	}
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
	switch (item->getType())
	{
	case Normal:
		countN--;
		break;
	case VIP:
		countV--;
		break;
	case Special:
		countS--;
		break;
	}
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

int Truck::getCountS()
{
	return countS;
}

int Truck::getCountV()
{
	return countV;
}
int Truck::getCountN()
{
	return countN;
}

Cargo* Truck::getDifferentCargo(Type t)
{
	Cargo* cargo, *chosenCargo;
	LinkedQueue<Cargo*> tempQ;
	while(MovingC.dequeue(chosenCargo))
	{
		if (chosenCargo->getType() == t)
		{
			while (tempQ.dequeue(cargo))
				MovingC.enqueue(cargo, cargo->getDeliveryDistance());
			return chosenCargo;
		}
		tempQ.enqueue(chosenCargo);
	}
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