#pragma once
#include "UI.h"
#include "../ADTs/LinkedNormal.h"
#include "../ADTs/LinkedQueue.h"
#include "Event.h"
#include "Truck.h"
#include "Cargo.h"

class Event;
class UI;
class Truck;
class Cargo;

class Company
{
	int numOfMovingCargos;
	int NTcount;
	int STcount;
	int VTcount;
	int autoP;
	int maxW;
	int TripsBeforeCheckup;
	Time Clock;
	LinkedQueue<Event*> EventList;

	LinkedNormal WaitingNC;
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
	void RemoveWaitingNormal(int, Cargo*&);

	void PrintWaitingNC();
	void PrintWaitingSC();
	void PrintWaitingVC();
	void PrintDeliveredNC();
	void PrintDeliveredSC();
	void PrintDeliveredVC();
	void PrintWaitingNT();
	void PrintWaitingST();
	void PrintWaitingVT();
	void PrintMovingT();
	void PrintLoadingT();
	void PrintTrucksInMaintenance();

	int getLengthOfLists(int &LT, int &ET, int &MC, int &ICT, int &DC);
	Time GetClock() const;
	void FileLoading(const string filename);
	void Simulate();
	void Print();
	~Company();
};