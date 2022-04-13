#pragma once
#include "Event.h"

class CancelEvent : public Event
{
	char Type;
	int Dist;
	Time LoadTime;
	int Cost;

public:

	CancelEvent();

	void Execute();

	virtual ~CancelEvent();
};