#pragma once
#include "Event.h"

class PreparationEvent: public Event
{
	char Type;
	int Dist;
	Time LoadTime;
	int Cost;

public:

	PreparationEvent();

	void Execute() ;

	virtual ~PreparationEvent();
};