#include "../Header Files/Company.h"

void Company::AppendWaiting(Cargo* newCargo)
{
	type Type = newCargo.getType();
	switch(Type)
	case 1:
		WaitingNormal.insert(newCargo);
	case 2:
		WaitingSpecial.enqueue(newCargo);
	case 3:
		WaitingVIP.enqueue(newCargo);

}

void Company::RemoveWaitingNormal(int ID)
{
	int size = WaitingNormal.getLength();
	Cargo* Cargoptr;
	for(int i=0;i<size;i++){
		WaitingNormal.getEntry(i, Cargoptr);
		if (Cargoptr->getID() == ID)
		{
			WaitingNormal.remove(i);
			break;
		}
	}
}
