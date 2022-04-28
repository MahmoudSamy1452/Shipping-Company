
#include <string>
#include <fstream>
#include "../DEFS.h"
#include "../Header Files/Company.h"
#include "../Header Files/PreparationEvent.h"
#include "../Header Files/CancelEvent.h"
#include "../Header Files/PromoteEvent.h"


Company::Company()
{
	numOfMovingCargos = 0;
	interface_ = new UI(this);
}


void Company::AppendWaiting(Cargo* newCargo)
{
	Type type = newCargo->getType();
	switch (type)
	{
	case 2:
		WaitingNC.insert(WaitingNC.getLength()+1, newCargo);
		break;
	case 1:
		WaitingSC.enqueue(newCargo);
		break;
	default:
		WaitingVC.enqueue(newCargo, newCargo->getPriority());
	}
}

bool Company::RemoveWaitingNormal(int ID, Cargo* &removedCargo)
{
	int i = WaitingNC.search(ID, removedCargo);
	return WaitingNC.remove(i);
}

int Company::getLengthOfLists(int &LT, int &ET, int &MC, int &ICT, int &DC)
{
	LT = LoadingT.getLength();
	ET = WaitingNT.getLength() + WaitingST.getLength() + WaitingVT.getLength();
	Truck* t;
	LinkedQueue<Truck*> temp;
	MC = numOfMovingCargos;
	ICT = TrucksInMaintenance.getLength();
	DC = DeliveredNC.getLength() + DeliveredSC.getLength() + DeliveredVC.getLength();
	return (WaitingNC.getLength() + WaitingSC.getLength() + WaitingVC.getLength());
}

Time Company::GetClock() const
{
	return Clock;
}

void Company::FileLoading(string filename)
{
	//temporary
	int NTspeed, STspeed, VTspeed;

	int NTcap, STcap, VTcap;

	int NTtrips, STtrips, VTtrips;

	ifstream inFile;
	// input name
	inFile.open(filename);
	while(!inFile.is_open())
	{
		interface_->PrintErrorMessage();
		filename = interface_->readFilename();
		inFile.open(filename);
	}
	if (inFile.is_open())
	{
		inFile >> NTcount >> STcount >> VTcount;
		inFile >> NTspeed >> STspeed >> VTspeed;
		inFile >> NTcap >> STcap >> VTcap;
		inFile >> TripsBeforeCheckup >> NTtrips >> STtrips >> VTtrips;

		// create trucks

		for (int i = 0; i < NTcount; i++)
		{
			Truck* t = new Truck(Normal, NTcap, NTtrips, NTspeed);
			WaitingNT.enqueue(t);
		}

		for (int i = 0; i < STcount; i++)
		{
			Truck* t = new Truck(Special, STcap, STtrips, STspeed);
			WaitingST.enqueue(t);
			
		}

		for (int i = 0; i < VTcount; i++)
		{
			Truck* t = new Truck(VIP, VTcap, VTtrips, VTspeed);
			WaitingVT.enqueue(t);
		}

		inFile >> autoP >> maxW;

		//Events

		int numOfEvents, day, hours, id, dist, LT, cost;
		char eventType, cargoType, temp;

		inFile >> numOfEvents;
		
		for (int i = 0; i < numOfEvents; i++)
		{
			
			inFile >> eventType;
			if (eventType == 'R')
			{
				inFile >> cargoType;
				inFile >> day >> temp >> hours >> id >> dist >> LT >> cost;
				Time eTime(day, hours);
				if (cargoType == 'N')
				{
					Event* ev = new PreparationEvent(Normal, eTime, id, dist, LT, cost, this);
					EventList.enqueue(ev);

				}
				else if (cargoType == 'S')
				{
					Event* ev = new PreparationEvent(Special, eTime, id, dist, LT, cost, this);
					EventList.enqueue(ev);

				}
				else if (cargoType == 'V')
				{
					Event* ev = new PreparationEvent(VIP, eTime, id, dist, LT, cost, this);
					EventList.enqueue(ev);
				}
			}
			else if (eventType == 'X')
			{
				inFile >> day >> temp >> hours >> id;
				Time eTime(day, hours);
				Event* ev = new CancelEvent(eTime, id, this);
				EventList.enqueue(ev);
			}
			else if (eventType == 'P')
			{
				inFile >> day >> temp >> hours >> id >> cost;
				Time eTime(day, hours);
				Event* ev = new PromoteEvent(eTime, id, cost, this);
				EventList.enqueue(ev);
			}
		}
	}
}

void Company::Simulate()
{
	string filename =  interface_->readFilename();
	FileLoading(filename);
	interface_->readInterfaceMode();
	interface_->wait();
	
	Event* eve;
	Cargo* removed;
	int count = 0;
	if (interface_->getUImode() == Silent)
	{
		interface_->StartSilent();
	}
	while(!EventList.isEmpty() || !WaitingNC.isEmpty() || !WaitingSC.isEmpty() || !WaitingVC.isEmpty())
	{
		EventList.peek(eve);
		while (eve->getTime() == Clock && !EventList.isEmpty())
		{
			eve->Execute();
			EventList.dequeue(eve);
			delete eve;
			EventList.peek(eve);
		}

		//Check if any Normal Cargo reached autoP and promotes it
		Cargo* C;
		for(int i = 1; i < WaitingNC.getLength() + 1; i++)
		{
			WaitingNC.getEntry(i, C);
			if (C->getWaitingTime(Clock) == autoP*24)
			{
				Event* autoPAction = new PromoteEvent(Clock, C->getID(), 0, this);
				autoPAction->Execute();
				delete autoPAction;
			}
		}


		if(count % 5 == 0 && count != 0)
		{
			if (WaitingNC.getEntry(1, removed))
			{
				DeliveredNC.enqueue(removed);
				WaitingNC.remove(1);
			}
			if(WaitingSC.dequeue(removed))
				DeliveredSC.enqueue(removed);
			if(WaitingVC.dequeue(removed))
				DeliveredVC.enqueue(removed);
		}
		if (interface_->getUImode() != Silent)
		{
			interface_->PrintHour();
		}
		Clock.incrementTime();
		count++;
	}
	cout << "Simulation ends, Output file created\n";
}

void Company::PrintWaitingNC()
{
	WaitingNC.Print();
}

void Company::PrintWaitingSC()
{
	WaitingSC.Print();
}

void Company::PrintWaitingVC()
{
	WaitingVC.Print();
}

void Company::PrintDeliveredNC()
{
	DeliveredNC.Print();
}

void Company:: PrintDeliveredSC()
{
	DeliveredSC.Print();
}

void Company::PrintDeliveredVC()
{
	DeliveredVC.Print();
}

void Company::PrintWaitingNT()
{
	WaitingNT.Print();
}

void Company::PrintWaitingST()
{
	WaitingST.Print();
}

void Company::PrintWaitingVT()
{
	WaitingVT.Print();
}

void Company::PrintMovingT()
{
	MovingT.Print();
}

void Company::PrintLoadingT()
{
	LoadingT.Print();
}

void Company::PrintTrucksInMaintenance()
{
	TrucksInMaintenance.Print();
}

Company::~Company()
{
	delete interface_;
}