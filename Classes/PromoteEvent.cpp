#include "../Header Files/PromoteEvent.h"

void PromoteEvent::Execute()
{
	Cargo* promotedCargo = RemoveWaitingNormal(ID);
	promotedCargo->setType("VIP");
	AppendWaiting(promotedCargo);
}
