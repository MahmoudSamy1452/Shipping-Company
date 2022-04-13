#pragma once
#include "Event.h"

class PromoteEvent : public Event
{
	int ExtraMoney;

public:

	PromoteEvent();

	void Execute();

	virtual ~PromoteEvent();
};