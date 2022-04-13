#include "Time.h"
#include "Company.h"

class Event
{
	Company* pCompany;
	Time EvtTime;
	int ID;

public:

	Event(int, int, int, Company*);

	virtual void Execute() = 0;

	virtual ~Event();
};