#include "../Header Files/Company.h"
#include "../DEFS.h"
#include <fstream>
#include "../Header Files/Truck.h"

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

void Company::FileLoading(string filename)
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
	if(inFile.is_open())
	{
		inFile >> NTcount >> STcount >> VTcount;
		inFile >> NTspeed>>STspeed >> VTspeed;
		inFile>>NTcap>>STcap>>VTcap;
		inFile >> TripsBeforeCheckup >> NTtrips >> STtrips >> VTtrips;

		// create trucks
		
		for(int i=0;i<NTcount;i++)
		{
			Truck* t = new Truck(Normal, NTcap, NTtrips, NTspeed, i);
			//put in list
		}

		for(int i=0;i<STcount;i++)
		{
			Truck* t = new Truck(Special, STcap, STtrips, STspeed, i);
			//put in list
		}

		for(int i=0;i<NTcount;i++)
		{
			Truck* t = new Truck(VIP, VTcap, VTtrips, VTspeed, i);
			//put in list
		}

		inFile >> autoP >> maxW >> eventCount;
		
	}

}

void Company::Simulate()
{

}

Company::~Company()
{
	
}