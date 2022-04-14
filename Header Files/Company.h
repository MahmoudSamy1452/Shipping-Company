#pragma once
#include "../ADTs/LinkedList.h"
#include "../ADTs/LinkedQueue.h"
#include "../ADTs/PriorityQueue.h"
#include "Event.h"
#include "Cargo.h"

class Event;

class Company
{
	int NTcount;
	int STcount;
	int VTcount;
	int autoP;
	int maxW;
	int TripsBeforeCheckup;
	LinkedQueue<Event*> EventList;
	LinkedList<Cargo*> WaitingNC;
	LinkedQueue<Cargo*> WaitingSC;
	PriorityQueue<Cargo*> WaitingVC;
	
	public:

	Company();

	void AppendWaiting(Cargo*);
	bool RemoveWaitingNormal(int, Cargo*&);
	void FileLoading(string filename);
	void Simulate();
	~Company();
};