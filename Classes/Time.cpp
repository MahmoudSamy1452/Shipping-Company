#include "../Header Files/Time.h"

Time::Time(int day = 1, int hour = 0):Day(day), Hour(hour)
{
}

void Time::incrementTime()
{
	if (++Hour >= 24)
	{
		Hour = 0;
		Day++;
	}
}

bool Time::isTime(Time t)
{
	if (Day == t.Day && Hour == t.Hour)
		return true;
	return false;
}
