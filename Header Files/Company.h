#pragma once
#include "UI.h"
#include "../ADTs/LinkedList.h"
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/PriorityQueue.h"
#include "Event.h"
#include "Cargo.h"
#include "Truck.h"

class Event;

class Company
{
	int NTcount;
	int STcount;
	int VTcount;
	int autoP;
	int maxW;
	int TripsBeforeCheckup;
	Time Clock;
	LinkedQueue<Event*> EventList;

	LinkedList<Cargo*> WaitingNC;
	LinkedQueue<Cargo*> WaitingSC;
	PriorityQueue<Cargo*> WaitingVC;

	LinkedQueue<Cargo*> DeliveredNC;
	LinkedQueue<Cargo*> DeliveredSC;
	LinkedQueue<Cargo*> DeliveredVC;

	LinkedQueue<Truck*> WaitingNT;
	LinkedQueue<Truck*> WaitingST;
	LinkedQueue<Truck*> WaitingVT;

	PriorityQueue<Truck*> MovingT;

	PriorityQueue<Truck*> LoadingT;

	PriorityQueue<Truck*> TrucksInMaintenance;

	UI* interface;

	public:

	Company();

	void AppendWaiting(Cargo*);
	bool RemoveWaitingNormal(int, Cargo*&);
	void FileLoading(const string filename);
	void Simulate();
	void Print();
	~Company();
};