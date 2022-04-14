#include "../Header Files/Time.h"

Time::Time(int day, int hour): Day(day), Hour(hour)
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

bool Time::isTime(Time t) const
{
	if (Day == t.Day && Hour == t.Hour)
		return true;
	return false;
}

int Time::toInt() const
{
	int num{ 0 };
	num = Day * 24 + this->Hour;
	return num;
}

