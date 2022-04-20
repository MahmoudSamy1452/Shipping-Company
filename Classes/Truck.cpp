#include "../Header Files/Truck.h"
#include "../ADTs/LinkedQueue.h"
#include <string>

int Truck::currtruckID = 1;

Truck::Truck(Type truckType, int capacity, int maintenanceTime, int speed)
{
	this->numOfCargos = capacity; 
	this->truckType = truckType;
	this->capacity = capacity;
	this->maintenanceTime = maintenanceTime;
	this->deliveryJourneys = 0;
	this->speed = speed;
	deliveryInterval = 0;
	totalActiveTime = 0;
	truckID = currtruckID;
	currtruckID++;
}

int Truck::getID() const
{
	return truckID;
}

int Truck::getNoOfCargos() const
{
	return numOfCargos;
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

void Truck::PrintEmpty(string& str)
{
	str += to_string(truckID);
}

void Truck::PrintLoading(string& str)
{
	str += to_string(truckID);
	Cargo* C;
	pCargo.peek(C);
	Type cargotype = C->getType();
	switch(cargotype)
	{
	case VIP:
	{
		str += " {";
		LinkedQueue<Cargo*> tempQueue;
		for (int i = 0; i < numOfCargos; i++)
		{
			pCargo.dequeue(C);
			C->Print(str);
			tempQueue.enqueue(C);
			if (pCargo.getLength() == 0)
				break;
			str += ", ";
		}
		for (int i = 0; i < numOfCargos; i++)
		{
			tempQueue.dequeue(C);
			pCargo.enqueue(C, C->getPriority());
		}
		str += "}";
		break;
	}
	case Special:
	{
		str += " (";
		LinkedQueue<Cargo*> tempQueue;
		for (int i = 0; i < numOfCargos; i++)
		{
			pCargo.dequeue(C);
			C->Print(str);
			tempQueue.enqueue(C);
			if (pCargo.getLength() == 0)
				break;
			str += ", ";
		}
		for (int i = 0; i < numOfCargos; i++)
		{
			tempQueue.dequeue(C);
			pCargo.enqueue(C, C->getPriority());
		}
		break;
	}
	case Normal:
	{
		str += "[";
		LinkedQueue<Cargo*> tempQueue;
		for (int i = 0; i < numOfCargos; i++)
		{
			pCargo.dequeue(C);
			C->Print(str);
			tempQueue.enqueue(C);
			if (pCargo.getLength() == 0)
				break;
			str += ", ";
		}
		for (int i = 0; i < numOfCargos; i++)
		{
			tempQueue.dequeue(C);
			pCargo.enqueue(C, C->getPriority());
		}
		str += "]";
		break;
	}
	}
}

Truck::~Truck()
{
}