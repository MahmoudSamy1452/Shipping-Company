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

	while(!EventList.isEmpty() || numOfCargos != DeliveredNC.getLength()+DeliveredSC.getLength()+DeliveredVC.getLength() || !TrucksInMaintenance.isEmpty() || !MovingT.isEmpty())
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

		//Check if any trucks in maintenance are done to return them to waiting
		CheckCheckupTrucks();

		DeliverCargos();

		Assign();

		// Check MaxW for cargos loaded in trucks to move trucks immediately
		Truck* tempT;
		int n = LoadingT.getLength();
		for (int i = 0; i < n; i++)
		{
			LoadingT.dequeue(tempT);
			ChecktoMove(tempT, tempT->getMaxWaitingCargo(Clock) == maxW);
			if(tempT)
				LoadingT.enqueue(tempT);
		}

		if (interface_->getUImode() != Silent)
		{
			interface_->PrintHour();
		}
		Clock.incrementTime();
		count++;
	}
	interface_->End();
}

void Company::CheckCheckupTrucks()
{
	Truck* truck;
	while(TrucksInMaintenance.peek(truck) && truck->getfinalTime().isTime(Clock))
	{
		truck->resetFinalTime();
		TrucksInMaintenance.dequeue(truck);
		truck->setStatus(Waiting);
		switch (truck->getType())
		{
		case Normal:
			WaitingNT.enqueue(truck, truck->getPriority());
			break;
		case Special:
			WaitingST.enqueue(truck, truck->getPriority());
			break;
		case VIP:
			WaitingVT.enqueue(truck, truck->getPriority());
			break;
		}
	}
}

void Company::Assign()
{
	Truck* LoadingN = nullptr;
	Truck* LoadingS = nullptr;
	Truck* LoadingV = nullptr;
	Truck* tempT = nullptr;
	Cargo* ctemp;

	//Getting Trucks to be loaded from LoadingT for each type if available, otherwise would take a new truck from the waiting lists
	int numofLoadingT = LoadingT.getLength();
	for(int i = 0; i < numofLoadingT; i++)
	{
		LoadingT.dequeue(tempT);
		if (tempT->isFull())
		{
			ChecktoMove(tempT, false);
			continue;
		}
		
		switch (tempT->getType())
		{
		case Normal:
			LoadingN = tempT;
			break;
		case Special:
			LoadingS = tempT;
			break;
		case VIP:
			LoadingV = tempT;
			break;
		}
	}

	if (!LoadingN)
		WaitingNT.dequeue(LoadingN);
	if (!LoadingS)
		WaitingST.dequeue(LoadingS);
	if (!LoadingV)
		WaitingVT.dequeue(LoadingV);

	//Check for maxW cargos before normal ones
	AssignMaxW(LoadingN, LoadingS, LoadingV);

	if (LoadingV && !LoadingV->getNoOfCargos()) 
	{
		numofLoadingT = LoadingT.getLength();
		for (int i = 0; i < numofLoadingT; i++)
		{
			LoadingT.dequeue(tempT);
			if (tempT->getType() == VIP)
			{
				LoadingT.enqueue(tempT);
				continue;
			}
			for (int i = 0; i < tempT->getCountV(); i++)
				if(AssignVIP(tempT->getDifferentCargo(Normal), LoadingV, LoadingN, LoadingS, false))
					i--;
			if (tempT->getNoOfCargos())
				LoadingT.enqueue(tempT);
			else
			{
				tempT->resetFinalTime();
				WaitingVT.enqueue(tempT, tempT->getPriority());
			}
		}
		if (LoadingN)
		{
			for (int i = 0; i < LoadingN->getCountV(); i++)
				if (AssignVIP(LoadingN->getDifferentCargo(VIP), LoadingV, LoadingN, LoadingS, false))
					i--;
			if (!LoadingV->getNoOfCargos())
			{
				WaitingVT.enqueue(LoadingV, LoadingV->getPriority());
				WaitingVT.dequeue(LoadingV);
			}
		}
		if (LoadingS)
		{
			for (int i = 0; i < LoadingS->getCountV(); i++)
				if (AssignVIP(LoadingS->getDifferentCargo(VIP), LoadingV, LoadingN, LoadingS, false))
					i--;
			if (!LoadingS->getNoOfCargos())
			{
				WaitingST.enqueue(LoadingS, LoadingS->getPriority());
				WaitingST.dequeue(LoadingS);
			}
		}
	}

	if (LoadingN && !LoadingN->getNoOfCargos())
	{
		Truck* tempT;
		int numofLoadingT = LoadingT.getLength();
		for (int i = 0; i < numofLoadingT; i++)
		{
			LoadingT.dequeue(tempT);
			if (tempT->getType() != VIP)
			{
				LoadingT.enqueue(tempT);
				continue;
			}
			for (int i = 0; i < tempT->getCountN(); i++)
			{
				if(AssignNormal(tempT->getDifferentCargo(Normal), LoadingN, LoadingV, false))
					i--;
			}
			if(tempT->getNoOfCargos())
				LoadingT.enqueue(tempT);
			else
			{
				tempT->resetFinalTime();
				WaitingVT.enqueue(tempT, tempT->getPriority());
			}
		}
		if (LoadingV)
		{
			for (int i = 0; i < LoadingV->getCountN(); i++)
				if(AssignNormal(LoadingV->getDifferentCargo(Normal), LoadingN, LoadingV, false))
					i--;
			if (!LoadingV->getNoOfCargos())
			{
				WaitingVT.enqueue(LoadingV, LoadingV->getPriority());
				WaitingVT.dequeue(LoadingV);
			}
		}
	}

	//Normal Assignment
	while (WaitingVC.peek(ctemp) && AssignVIP(ctemp, LoadingV, LoadingN, LoadingS, false)) 
			WaitingVC.dequeue(ctemp);

	while (WaitingSC.peek(ctemp) && AssignSpecial(ctemp, LoadingS, false))
			WaitingSC.dequeue(ctemp);

	while (WaitingNC.getEntry(1, ctemp) && AssignNormal(ctemp, LoadingN, LoadingV, false))
			WaitingNC.remove(1);

	//Return trucks to loading and waiting lists for printing and to retrieve in the next hour again
	if (LoadingN && LoadingN->getNoOfCargos() > 0)
		LoadingT.enqueue(LoadingN);
	else if (LoadingN)
		WaitingNT.enqueue(LoadingN, LoadingN->getPriority());
	if (LoadingS && LoadingS->getNoOfCargos() > 0)
		LoadingT.enqueue(LoadingS);
	else if (LoadingS)
		WaitingST.enqueue(LoadingS, LoadingS->getPriority());
	if (LoadingV && LoadingV->getNoOfCargos() > 0)
		LoadingT.enqueue(LoadingV);
	else if (LoadingV)
		WaitingVT.enqueue(LoadingV, LoadingV->getPriority());
}

void Company::AssignMaxW(Truck*& LoadingN, Truck*& LoadingS, Truck*& LoadingV)
{
	Cargo* ctemp;
	LinkedQueue<Cargo*> temp;
	int waitingtime;

	//VIP cargos are not sorted by arrival time so we must check all cargos for maxW
	while (WaitingVC.dequeue(ctemp)) 
	{
		waitingtime = ctemp->getWaitingTime(Clock);
		if (!(waitingtime == maxW && AssignVIP(ctemp, LoadingV, LoadingN, LoadingS, true)))
			temp.enqueue(ctemp);
	}
	while (temp.dequeue(ctemp))
		WaitingVC.enqueue(ctemp, ctemp->getPriority());

	//Normal cargos that may reach maxW are in the front
	while (WaitingNC.getEntry(1, ctemp)) {
		waitingtime = ctemp->getWaitingTime(Clock);
		if (waitingtime == maxW && AssignNormal(ctemp, LoadingN, LoadingV, true))
				WaitingNC.remove(1);
		else
			break;
	}

	//Special cargos that may reach maxW are in the front
	while (WaitingSC.peek(ctemp)) {
		waitingtime = ctemp->getWaitingTime(Clock);
		if (waitingtime == maxW && AssignSpecial(ctemp, LoadingS, true))
				WaitingSC.dequeue(ctemp);
		else 
			break;
	}
}

bool Company::AssignVIP(Cargo* cargo, Truck*& LoadingVT, Truck*& LoadingNT, Truck*& LoadingST, bool isMaxW)
{
	if (LoadingVT)
	{
		LoadingVT->load(cargo, Clock);
		ChecktoMove(LoadingVT, isMaxW);
		if (!LoadingVT)
			WaitingVT.dequeue(LoadingVT);
	}
	else if (LoadingNT)
	{
		LoadingNT->load(cargo, Clock);
		ChecktoMove(LoadingNT, isMaxW);
		if (!LoadingNT)
			WaitingNT.dequeue(LoadingNT);
	}
	else if (LoadingST)
	{
		LoadingST->load(cargo, Clock);
		ChecktoMove(LoadingST, isMaxW);
		if (!LoadingST)
			WaitingST.dequeue(LoadingST);
	}
	else
		return false;
	return true;
}

bool Company::AssignNormal(Cargo* cargo,Truck*& LoadingNT,Truck*& LoadingVT, bool isMaxW)
{
	if (LoadingNT)
	{
		LoadingNT->load(cargo, Clock);
		ChecktoMove(LoadingNT, isMaxW);
		if (!LoadingNT)
			WaitingNT.dequeue(LoadingNT);
	}
	else if (LoadingVT) 
	{
		LoadingVT->load(cargo, Clock);
		ChecktoMove(LoadingVT, isMaxW);
		if (!LoadingVT)
			WaitingVT.dequeue(LoadingVT);
	}
	else
		return false;
	return true;
}



bool Company::AssignSpecial(Cargo* cargo, Truck*& LoadingST, bool isMaxW)
{
	if (LoadingST) 
	{
		LoadingST->load(cargo, Clock);
		ChecktoMove(LoadingST, isMaxW);
		if (!LoadingST)
			WaitingST.dequeue(LoadingST);
	}
	else 
		return false;
	return true;
}

void Company::DeliverCargos()
{
	Cargo* cargo;
	Truck* tempT;
	LinkedQueue<Truck*> tempQ;

	//Check if any cargos should be delivered now for all moving trucks
	while (MovingT.dequeue(tempT))
	{
		while (tempT->getFirstArrival().isTime(Clock))
			{
				cargo = nullptr;
				tempT->unload(cargo);
				cargo->setCDT(Clock);
				switch (cargo->getType())
				{
				case Normal:
					DeliveredNC.enqueue(cargo);
					break;
				case Special:
					DeliveredSC.enqueue(cargo);
					break;
				case VIP:
					DeliveredVC.enqueue(cargo);
					break;
				}
				numOfMovingCargos--;
			}
		tempQ.enqueue(tempT);
	}

	//Return moving truck to waiting list if became empty or back to movingT if still has cargos
	while (tempQ.dequeue(tempT))
	{
		if (tempT->getfinalTime().toInt() > Clock.toInt())
			MovingT.enqueue(tempT, tempT->getfinalTime().toInt());
		else
		{
			tempT->resetFinalTime();
			tempT->incrementJourneys();
			if (tempT->getDeliveryJourneys() == TripsBeforeCheckup)
			{
				tempT->resetJourneys();
				tempT->setStatus(Maintenance);
				TrucksInMaintenance.enqueue(tempT, tempT->calculatefinaltime(Clock));
				continue;
			}
			switch (tempT->getType())
			{
			case Normal:
				WaitingNT.enqueue(tempT, tempT->getPriority());
				break;
			case Special:
				WaitingST.enqueue(tempT, tempT->getPriority());
				break;
			case VIP:
				WaitingVT.enqueue(tempT, tempT->getPriority());
				break;
			}
		}
	}
}

void Company::ChecktoMove(Truck*& truck, bool isMaxW)
{
	if (truck && truck->getfinalTime().isTime(Clock))
	{
		truck->resetLoad();
		truck->resetFinalTime();
		truck->setStatus(Moving);
		truck->setdeliveryInterval();
		truck->setMoveTime(Clock);
		MovingT.enqueue(truck, truck->calculatefinaltime(Clock));
		numOfMovingCargos += truck->getNoOfCargos();
		truck = nullptr;
		return;
	}
	truck->setStatus(Loading);
	if (truck->isFull() || isMaxW)
	{
		if (!truck->getfinalTime().isValid())
		{
			truck->calculatefinaltime(Clock);
		}
		LoadingT.enqueue(truck);
		truck = nullptr;
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

