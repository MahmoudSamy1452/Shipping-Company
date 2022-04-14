#include "../Header Files/PreparationEvent.h"
#include "../Header Files/Company.h"

PreparationEvent::PreparationEvent(Type cargoType, Time evtTime, int ID, int dist, Time loadTime, int cost, Company* pCompany): Event(evtTime, ID,  pCompany), loadTime(loadTime)
{
	this->cargotype = cargoType;
	this->dist = dist;
	this->cost = cost;

}

void PreparationEvent::Execute()
{
	Cargo* newCargo = new Cargo(cargotype, ID, dist, loadTime, evtTime.toInt(), cost);
	pCompany->AppendWaiting(newCargo);
}
