#include "../Header Files/Company.h"
#include "../DEFS.h"
#include <fstream>
#include "../Header Files/Truck.h"
#include "../Header Files/Event.h"
#include "../Header Files/PreparationEvent.h"
#include "../Header Files/CancelEvent.h"
#include "../Header Files/PromoteEvent.h"
#include <string>



Company::Company()
{
}


void Company::AppendWaiting(Cargo* newCargo)
{
	Type type = newCargo->getType();
	switch (type)
	{
	case 2:
		WaitingNC.insert(WaitingNC.getLength(), newCargo);
	case 1:
		WaitingSC.enqueue(newCargo);
	default:
		WaitingVC.enqueue(newCargo, newCargo->getPriority());
	}

}

bool Company::RemoveWaitingNormal(int ID, Cargo* &removedCargo)
{
	int size = WaitingNC.getLength();
	for(int i=0;i<size;i++){
		WaitingNC.getEntry(i, removedCargo);
		if (removedCargo->getID() == ID)
		{
			WaitingNC.remove(i);
			return true;
		}
	}
	return false;
}

void Company::FileLoading(const string filename)
{
	//temporary
	int NTspeed;
	int STspeed;
	int VTspeed;

	int NTcap;
	int STcap;
	int VTcap;

	int NTtrips;
	int STtrips;
	int VTtrips;

	int eventCount;

	ifstream inFile;
	// input name
	inFile.open(filename);
	if (inFile.is_open())
	{
		inFile >> NTcount >> STcount >> VTcount;
		inFile >> NTspeed >> STspeed >> VTspeed;
		inFile >> NTcap >> STcap >> VTcap;
		inFile >> TripsBeforeCheckup >> NTtrips >> STtrips >> VTtrips;

		// create trucks

		for (int i = 0; i < NTcount; i++)
		{
			Truck* t = new Truck(Normal, NTcap, NTtrips, NTspeed, i);
			//put in list
		}

		for (int i = 0; i < STcount; i++)
		{
			Truck* t = new Truck(Special, STcap, STtrips, STspeed, i);
			//put in list
		}

		for (int i = 0; i < NTcount; i++)
		{
			Truck* t = new Truck(VIP, VTcap, VTtrips, VTspeed, i);
			//put in list
		}

		inFile >> autoP >> maxW >> eventCount;

		//Events

		int numOfEvents{ 0 };
		inFile >> numOfEvents;
		for (int i = 0; i < numOfEvents; i++)
		{
			char eventType{ 0 };
			char cargoType{ 0 };
			int day{ 0 };
			int hours{ 0 };
			int id{ 0 };
			int dist{ 0 };
			int LT{ 0 };
			int cost{ 0 };
			inFile >> eventType;
			inFile >> cargoType;
			inFile >> day >> hours >> id >> dist >> LT >> cost;
			Time eTime(day, hours);
			if (eventType == 'R')
			{
				if (cargoType == 'N')
				{
					PreparationEvent ev(Normal, eTime, id, dist, LT, cost, this);
					EventList.enqueue(&ev);

				}
				else if (cargoType == 'S')
				{
					PreparationEvent ev(Special, eTime, id, dist, LT, cost, this);
					EventList.enqueue(&ev);

				}
				else if (cargoType == 'V')
				{
					PreparationEvent ev(VIP, eTime, id, dist, LT, cost, this);
					EventList.enqueue(&ev);

				}
			}
			else if (eventType == 'X')
			{

				CancelEvent ev(eTime, id, this);
				EventList.enqueue(&ev);

			}
			else if (eventType == 'P')
			{
				PromoteEvent ev(eTime, id, cost, this);
				EventList.enqueue(&ev);

			}

		}
	}
}

void Company::Simulate()
{
	UI Interface;
	string filename = Interface.readFilename();
	FileLoading(filename);
	Event* eve;
	Cargo* removed;
	int count = 0;
	while(!EventList.isEmpty() || !WaitingNC.isEmpty() || !WaitingSC.isEmpty() || !WaitingVC.isEmpty()){
		EventList.peek(eve);
		if (eve->getTime() == Clock){
			eve->Execute();
		}
		if(count % 5 == 0 && count != 0){
			if (WaitingNC.getEntry(1, removed)){
				DeliveredNC.enqueue(removed);
				WaitingNC.remove(1);
			}
			if(WaitingSC.dequeue(removed))
				DeliveredSC.enqueue(removed);
			if(WaitingVC.dequeue(removed))
				DeliveredVC.enqueue(removed);
		}
			Clock.incrementTime();
			count++;
	}
}

Company::~Company()
{
	
}