#include "../DEFS.h"
#include "../Header Files/Cargo.h"
#include <string>

Cargo::Cargo(Type cargoType,int ID, int deliveryDistance, Time readyTime, int loadingTime,  int cost)
{
	this->ID = ID;
	this->cargoType = cargoType;
	this->deliveryDistance = deliveryDistance;
	this->cost = cost;
	this->ready_time = readyTime;
	delivered = false;
	assigned = false;
	this->loading_time = loadingTime;
}

void Cargo::Cargo::setType(Type cargoType)
{
	this->cargoType = cargoType;
}

void Cargo::setDeliveryTime(int t)
	{
		deliveryTime = t;
	}

	void Cargo::addExtraMoney(int money)
	{
		cost += money;
	}

	int Cargo::getDeliveryTime()
	{
		return deliveryTime;
	}

Type Cargo::getType()
{
	return cargoType;
}

void Cargo::setDeliveryDistance(float deliveryDistance)
{
	this->deliveryDistance = deliveryDistance;
}

float Cargo::getDeliveryDistance()
{
	return deliveryDistance;
}

void Cargo::setCost(float cost)
{
	this->cost = cost;
}

float Cargo::getCost()
{
	return cost;
}

bool Cargo::isDelivered()
{
	return delivered;
}

bool Cargo::isAssigned()
{
	return assigned;
}

void Cargo::setDeliveryStatus(bool status)
{
	delivered = status;
}

void Cargo::setAssignmentStatus(bool status)
{
	assigned = status;
}

void Cargo::setMoveTime(Time t)
{
	moveTime = t;
}

Time Cargo::getMoveTime()
{
	return moveTime;
}

void Cargo::setReadyTime(Time t)
{
	this->ready_time = t;
}

void Cargo::setLoadingTime(Time t)
{
	this->loading_time = t.toInt();
}

Time Cargo::getReadyTime()
{
	return ready_time;
}

int Cargo::getLoadingTime()
{
	return loading_time;
}

int Cargo::getPriority()
{
	if (cargoType != VIP)
	{
		return 0;
	}
	int priority = ready_time.toInt() + deliveryDistance * 2 + cost * 3 + loading_time * 2;
	return priority;
}

void Cargo::setID(int ID)
{
	this->ID = ID;
}

int Cargo::getID() const
{
	return ID;
}

Time Cargo::getWaitingTime()
{
	return moveTime - ready_time;
}

void Cargo::Print(std::string& str){
	str += std::to_string(ID);
}
