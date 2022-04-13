#pragma once
#include "../ADTs/LinkedList.h"
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/PriorityQueue.h"
class Company
{
	int autoP;
	int maxW;
	LinkedList<Cargo*> WaitingNormal;
	LinkedQueue<Cargo*> WaitingSpecial;
	PriorityQueue<Cargo*> WaitingVIP;
	
	public:
	void AppendWaiting(Cargo* newCargo);

	void RemoveWaitingNormal(int ID);
};