#include "../DEFS.h"
#include "../Header Files/Cargo.h"

Cargo::Cargo(Type type,int ID, int deliveryDistance, Time readyTime, int loadingTime,  int cost)
{
	this->ID = ID;
	this->type = type;
	this->deliveryDistance = deliveryDistance;
	this->cost = cost;
	this->ready_time = readyTime;
	delivered = false;
	assigned = false;
	loading_time = loadingTime;
}

void Cargo::setType(Type type)
{
	this->type = type;
}

Type Cargo::getType()
{
	return type;
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

int Cargo::getWaitingTime(Time t)
{
	return (t.toInt() - ready_time.toInt());
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

Time Cargo::getLoadingTime()
{
	return loading_time;
}

int Cargo::getPriority()
{
	if (type != VIP)
	{
		return 0;
	}
	int priority{ 0 };
	priority = (ready_time.toInt() * deliveryDistance) / (cost * loading_time);
	return priority;
}

void Cargo::setID(int ID)
{
	this->ID = ID;
}

int Cargo::getID()
{
	return ID;
}
