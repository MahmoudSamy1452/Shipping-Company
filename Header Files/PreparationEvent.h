#pragma once
#include "Event.h"
#include "Time.h"

class PreparationEvent: public Event
{
	Type cargotype;
	int dist;
	Time loadTime;
	int cost;

public:

	PreparationEvent(Type, Time, int, int, Time, int, Company*);
	virtual void Execute();
};