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
	interface = new UI;
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
	int size = WaitingNC.getLength();
	for(int i=0;i<size;i++){
		WaitingNC.getEntry(i+1, removedCargo);
		if (removedCargo->getID() == ID)
		{
			WaitingNC.remove(i+1);
			return true;
		}
	}
	return false;
}

void Company::FileLoading(const string filename)
{
	//temporary
	int NTspeed, STspeed, VTspeed;

	int NTcap, STcap, VTcap;

	int NTtrips, STtrips, VTtrips;

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
	string filename =  interface->readFilename();
	FileLoading("Test1.txt");
	Event* eve;
	Cargo* removed;
	int count = 0;
	while(!EventList.isEmpty() || !WaitingNC.isEmpty() || !WaitingSC.isEmpty() || !WaitingVC.isEmpty())
	{
		EventList.peek(eve);
		while (eve->getTime() == Clock && !EventList.isEmpty())
		{
			eve->Execute();
			EventList.dequeue(eve);
			EventList.peek(eve);
		}

		//Check if any Normal Cargo reached autoP and promotes it
		Cargo* C;
		for(int i = 1; i < WaitingNC.getLength() + 1; i++)
		{
			WaitingNC.getEntry(i, C);
			if (C->getWaitingTime(Clock) == autoP)
			{
				WaitingNC.remove(i);
				C->setType(VIP);
				WaitingVC.enqueue(C, C->getPriority());
			}
		}
		/*if(count % 5 == 0 && count != 0)
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
		}*/
		WaitingNC.Print();
		interface->wait();
		Clock.incrementTime();
		count++;
	}
}

//void Company::Print()
//{
//	int WaitingCargos = WaitingNC.getLength() + WaitingSC.getLength() + WaitingVC.getLength();
//	string str = "\nCurrent time (Day:Hour): ";
//	interface->PrintString(str);
//	interface->printTime(Clock);
//	str = "\n";
//	Cargo* C;
//	str += to_string(WaitingCargos); 
//	str += " Waiting Cargos: ";
//	str += "[";
//	for (int i  = 0; i < WaitingNC.getLength(); i++)
//	{
//		WaitingNC.getEntry(i+1,C);
//		C->Print(str);
//		if (i == WaitingNC.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	str += "]";
//	str += " (";
//	for (int i  = 0; i < WaitingSC.getLength(); i++)
//	{
//		WaitingSC.dequeue(C);
//		C->Print(str);
//		WaitingSC.enqueue(C);
//		if (i == WaitingSC.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	str += ")";
//	str += " {";
//	LinkedQueue<Cargo*> tempQueue;
//	for (int i  = 0; i < WaitingVC.getLength() + tempQueue.getLength(); i++)
//	{
//		WaitingVC.dequeue(C);
//		C->Print(str);
//		tempQueue.enqueue(C);
//		if (WaitingVC.getLength() == 0)
//			break;
//		str += ", ";
//	}
//	for (int i = 0;i < tempQueue.getLength() + WaitingVC.getLength();i++)
//	{
//		tempQueue.dequeue(C);
//		WaitingVC.enqueue(C, C->getPriority());
//	}
//	str += "}";
//	interface->PrintBreakLine();
//	////////////////////////////////////////////////////////////////////////////////////
//	Truck* T;
//	int LoadingTrucks = LoadingNT.getLength() + LoadingST.getLength() + LoadingVT.getLength();
//	str += to_string(LoadingTrucks); 
//	str += " Loading Trucks: ";
//	for (int i=0;i<LoadingNT.getLength();i++){
//		LoadingNT.dequeue(T);
//		T->PrintLoading(str);
//		LoadingNT.enqueue(T);
//		if (i == LoadingNT.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	for (int i=0;i<LoadingST.getLength();i++){
//		LoadingST.dequeue(T);
//		T->PrintLoading(str);
//		LoadingST.enqueue(T);
//		if (i == LoadingST.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	for (int i=0;i<LoadingVT.getLength();i++){
//		LoadingVT.dequeue(T);
//		T->PrintLoading(str);
//		LoadingVT.enqueue(T);
//		if (i == LoadingVT.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	interface->PrintBreakLine();
//	/////////////////////////////////////////////////////////////////////////////////
//	int EmptyTrucks = WaitingNT.getLength() + WaitingST.getLength() + WaitingVT.getLength();
//	str += to_string(EmptyTrucks); 
//	str += " Empty Trucks: ";
//	str += "[";
//	for (int i  = 0; i < WaitingNT.getLength(); i++)
//	{
//		WaitingNT.dequeue(T);
//		T->PrintEmpty(str);
//		if (i == WaitingNT.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	str += "]";
//	str += " (";
//	for (int i  = 0; i < WaitingST.getLength(); i++)
//	{
//		WaitingST.dequeue(T);
//		T->PrintEmpty(str);
//		WaitingST.enqueue(T);
//		if (i == WaitingST.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	str += ")";
//	str += " {";
//	for (int i  = 0; i < WaitingVT.getLength(); i++)
//	{
//		WaitingVT.dequeue(T);
//		T->PrintEmpty(str);
//		WaitingVT.enqueue(T);
//		if (i == WaitingVT.getLength() - 1)
//			break;
//		str += ", ";
//	}
//	str += "}";
//	interface->PrintString(str);
//	interface->PrintBreakLine();
//	///////////////////////////////////////////////////////////////
//}

Company::~Company()
{
	
}