#pragma once
#include "UI.h"
#include "../ADTs/LinkedNormal.h"
#include "../ADTs/LinkedQueue.h"
#include "Event.h"
#include "Truck.h"

class Event;
class UI;
class Truck;
class Cargo;

class Company
{
	int numOfCargos;
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

	PriorityQueue<Truck*> WaitingNT;
	PriorityQueue<Truck*> WaitingST;
	PriorityQueue<Truck*> WaitingVT;

	PriorityQueue<Truck*> MovingT;

	LinkedQueue<Truck*> LoadingT;

	PriorityQueue<Truck*> TrucksInMaintenance;

	UI* interface_;

	public:

	Company();

	void AppendWaiting(Cargo*);
	bool RemoveWaitingNormal(int, Cargo*&);
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
	void IncrementCargos();
	void DecrementCargos();
	int getLengthOfLists(int &LT, int &ET, int &MC, int &ICT, int &DC);
	Time GetClock() const;
	void FileLoading(const string filename);
	void Simulate();
	void Assign();
	void AssignMaxW(Truck* &currN, Truck* &currS, Truck* &currV);
	bool AssignNormal(Cargo* cargo, Truck* &currNT, Truck* &currVT, bool isMaxW);
	bool AssignSpecial(Cargo* cargo, Truck* &currST, bool isMaxW);
	bool AssignVIP(Cargo* cargo, Truck* &currVT,Truck* &curNT,Truck* &currST,bool isMaxW);
	void DeliverCargos();
	bool ChecktoMove(Truck* truck, bool isMaxW);
	void CheckCheckupTrucks();
	~Company();
};