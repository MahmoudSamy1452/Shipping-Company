#pragma once
#include "../ADTs/LinkedList.h"
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/PriorityQueue.h"
#include "Event.h"
#include "Cargo.h"
#include "Truck.h"
#include "UI.h"

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

	LinkedQueue<Truck*> WaitingTruckNC;
	LinkedQueue<Truck*> WaitingTruckSC;
	LinkedQueue<Truck*> WaitingTruckVC;

	LinkedQueue<Truck*> MovingTruckNC;
	LinkedQueue<Truck*> MovingTruckSC;
	LinkedQueue<Truck*> MovingTruckVC;

	PriorityQueue<Truck*> TrucksinMaintenance;

	public:

	Company();

	void AppendWaiting(Cargo*);
	bool RemoveWaitingNormal(int, Cargo*&);
	void FileLoading(const string filename);
	void Simulate();
	~Company();
};