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
	this->loading_time = loadingTime;
}

void Cargo::setType(Type cargoType)
{
	this->cargoType = cargoType;
}

	void Cargo::setLoadedTime(Time t)
	{
		loaded_time = t;
	}

	Time Cargo::getLoadedTime()
	{
		return loaded_time;
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
	if (cargoType == VIP)
	{
		return (cost * 4 + deliveryDistance * 3 - ready_time.toInt() * 2 + loading_time) * -1;
	}

	return  0;
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
