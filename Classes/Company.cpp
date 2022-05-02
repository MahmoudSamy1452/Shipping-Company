#include <string>
#include <fstream>
#include "../DEFS.h"
#include "../Header Files/Company.h"
#include "../Header Files/PreparationEvent.h"
#include "../Header Files/CancelEvent.h"
#include "../Header Files/PromoteEvent.h"


Company::Company():Clock(1,0)
{
	numOfCargos = 0;
	numOfMovingCargos = 0;
	interface_ = new UI(this);
}

void Company::IncrementCargos()
{
	numOfCargos++;
}

void Company::DecrementCargos()
{
	numOfCargos = (numOfCargos == 0) ? 0 : numOfCargos--;
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
		inFile >> TripsBeforeCheckup >> NTtrips >> STtrips >> VTtrips;

		// create trucks

		for (int i = 0; i < NTcount; i++)
		{
			inFile >> NTspeed >> NTcap;
			Truck* t = new Truck(Normal, NTcap, NTtrips, NTspeed);
			WaitingNT.enqueue(t, t->getPriority());
		}

		for (int i = 0; i < STcount; i++)
		{
			inFile >> STspeed >> STcap;
			Truck* t = new Truck(Special, STcap, STtrips, STspeed);
			WaitingST.enqueue(t, t->getPriority());
			
		}

		for (int i = 0; i < VTcount; i++)
		{
			inFile >> VTspeed >> VTcap;
			Truck* t = new Truck(VIP, VTcap, VTtrips, VTspeed);
			WaitingVT.enqueue(t, t->getPriority());
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
	int count = 0;
	if (interface_->getUImode() == Silent)
	{
		interface_->StartSilent();
	}

	Truck* currN = nullptr;
	Truck* currS = nullptr;
	Truck* currV = nullptr;
	while(!EventList.isEmpty() || numOfCargos != DeliveredNC.getLength()+DeliveredSC.getLength()+DeliveredVC.getLength())
	{
		if (!currN)
			WaitingNT.dequeue(currN);
		if (!currS)
			WaitingST.dequeue(currS);
		if (!currV)
			WaitingVT.dequeue(currV);
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

		Assign(currN, currS, currV);

		// MaxW in loaded trucks
		Truck* tempT;
		for (int i = 0; i < LoadingT.getLength(); i++)
		{
			if (LoadingT.dequeue(tempT) && tempT->getMaxWaitingCargo(Clock) == maxW)
			{
				MovingT.enqueue(tempT, tempT->calculatefinaltime(Clock));
				numOfMovingCargos += tempT->getNoOfCargos();
				switch(tempT->getType())
				{
				case Normal:
					currN = nullptr;
					break;
				case Special:
					currS = nullptr;
					break;
				case VIP:
					currV = nullptr;
					break;
				}
				continue;
			}
			LoadingT.enqueue(tempT);
		}

		while(LoadingT.dequeue(tempT))
		{
			switch(tempT->getType())
			{
			case Normal:
				currN = tempT;
				break;
			case Special:
				currS = tempT;
				break;
			case VIP:
				currV = tempT;
				break;
			}
		}
		if (currN && !currN->getNoOfCargos()) 
		{
			WaitingNT.enqueue(currN, currN->getPriority());
			currN = nullptr;
		}
		else if(currN)
			LoadingT.enqueue(currN);
		if (currS && !currS->getNoOfCargos())
		{
			WaitingST.enqueue(currS, currS->getPriority());
			currS = nullptr;
		}
		else if(currS)
			LoadingT.enqueue(currS);
		if (currV && !currV->getNoOfCargos())
		{
			WaitingVT.enqueue(currV, currV->getPriority());
			currV = nullptr;
		}
		else if(currV)
			LoadingT.enqueue(currV);

		if (interface_->getUImode() != Silent)
		{
			interface_->PrintHour();
		}
		Clock.incrementTime();
		count++;
	}
	interface_->End();
}

bool Company::AssignNormal(Cargo* cargo,Truck*& currNT,Truck*& currVT, bool isMaxW)
{
		if (currNT)
		{
			WaitingNC.remove(1);
			currNT->load(cargo, Clock);
			if (currNT->isFull() || isMaxW) {
				currNT->setMoveTime(Clock);
				MovingT.enqueue(currNT, currNT->calculatefinaltime(Clock));
				WaitingNT.peek(currNT);
				numOfMovingCargos += currNT->getNoOfCargos();
			}
		}
		else if (currVT) {
			WaitingNC.remove(1);
			currVT->load(cargo, Clock);
			if (currVT->isFull() || isMaxW) {
				currVT->setMoveTime(Clock);
				MovingT.enqueue(currVT, currVT->calculatefinaltime(Clock));
				WaitingVT.peek(currVT);
				numOfMovingCargos += currVT->getNoOfCargos();
			}
		}
		else
			return false;
		return true;
}

bool Company::AssignSpecial(Cargo* cargo, Truck*& currST, bool isMaxW)
{
	if (currST) {
		currST->load(cargo, Clock);
		if (currST->isFull())
		{
			currST->setMoveTime(Clock);
			MovingT.enqueue(currST, currST->calculatefinaltime(Clock));
			numOfMovingCargos += currST->getNoOfCargos();
			WaitingST.peek(currST);
		}
	}
	else return false;
	return true;
}


bool Company::AssignVIP(Cargo* cargo, Truck*& currVT, Truck*& currNT, Truck*& currST, bool isMaxW)
{
	if (currVT)
	{
		WaitingVC.dequeue(cargo);
		currVT->load(cargo, Clock);
		if (currVT->isFull() || isMaxW)
		{
			WaitingVT.dequeue(currVT);
			currVT->setMoveTime(Clock);
			MovingT.enqueue(currVT, currVT->calculatefinaltime(Clock));
			numOfMovingCargos += currVT->getNoOfCargos();
		}
	}
	else if (currNT)
	{
		WaitingVC.dequeue(cargo);
		currNT->load(cargo, Clock);
		if (currNT->isFull() || isMaxW)
		{
			WaitingNT.dequeue(currNT);
			currNT->setMoveTime(Clock);
			MovingT.enqueue(currNT, currNT->calculatefinaltime(Clock));
			numOfMovingCargos += currNT->getNoOfCargos();
		}
	}
	else if (currST)
	{
		WaitingVC.dequeue(cargo);
		currST->load(cargo, Clock);
		if (currST->isFull() || isMaxW)
		{
			WaitingST.dequeue(currST);
			currST->setMoveTime(Clock);
			MovingT.enqueue(currST, currST->calculatefinaltime(Clock));
			numOfMovingCargos += currST->getNoOfCargos();
		}
	}
	else
		return false;
	return true;
}


void Company::Assign(Truck*& currN, Truck*& currS, Truck*& currV)
{
	Cargo* ctemp;
	Truck* tempT;
	AssignMaxW(currN, currS, currV);
	while(WaitingVC.peek(ctemp)){
		if (AssignVIP(ctemp, currV, currN, currS, false))
			WaitingVC.dequeue(ctemp);
		else
			break;
	}
	while(WaitingSC.peek(ctemp))
	{
		if(AssignSpecial(ctemp, currS, false))
			WaitingSC.dequeue(ctemp);
		else
		     break;
	}
	while(WaitingNC.getEntry(1, ctemp))
	{
		if (AssignNormal(ctemp, currN, currV, false))
			WaitingNC.remove(1);
		else
			break;
	}
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

void Company::AssignMaxW(Truck*& currN, Truck*& currS, Truck*& currV)
{
	Cargo* ctemp;
	LinkedQueue<Cargo*> temp;
	int waitingtime;

	//VIP
	while(WaitingVC.dequeue(ctemp)){
		waitingtime = ctemp->getWaitingTime(Clock);
		if (waitingtime == maxW)
			AssignVIP(ctemp, currV, currN, currS, true);
		else
			temp.enqueue(ctemp);
	}
	while (temp.dequeue(ctemp))
		WaitingVC.enqueue(ctemp, ctemp->getPriority());

	//Normal
	while(WaitingNC.getEntry(1, ctemp)){
	waitingtime = ctemp->getWaitingTime(Clock); 
	if (waitingtime == maxW) {
		AssignNormal(ctemp, currN, currV, true);
		WaitingNC.remove(1);
	}
	else
		break;
	}

	//Special
	while(WaitingSC.peek(ctemp)) {
		waitingtime = ctemp->getWaitingTime(Clock);
		if (waitingtime == maxW) {
			AssignSpecial(ctemp, currS, true);
			WaitingSC.dequeue(ctemp);
		}
		else break;
	}
}
