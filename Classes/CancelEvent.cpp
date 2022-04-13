#include "../Header Files/CancelEvent.h"

void CancelEvent::Execute()
{
	RemoveWaitingNormal(ID);
}
