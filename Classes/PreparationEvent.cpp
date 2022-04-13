#include "../Header Files/PreparationEvent.h"

PreparationEvent::PreparationEvent(): Event(0,0,0), LoadTime(0, 0)
{
}

void PreparationEvent::Execute()
{
	Cargo* newCargo = new Cargo;
	AppendWaiting(newCargo);
}
