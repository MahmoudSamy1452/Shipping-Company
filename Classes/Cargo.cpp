#include "../DEFS.h"
#include "../Header Files/Cargo.h"
#include <iostream>
#include <string>
using namespace std;


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

float Cargo::getCost()
{
	return cost;
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

int Cargo::getID() const
{
	return ID;
}

int Cargo::getWaitingTime(Time now)
{
	return now.toInt() - ready_time.toInt();
}

void Cargo::Print(std::string& str){
	str += std::to_string(ID);
}
