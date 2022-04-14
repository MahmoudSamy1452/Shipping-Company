#pragma once
class Time
{
	int Day;
	int Hour;

public:

	Time(int = 1, int = 0);

	void incrementTime();

	bool isTime(Time) const;

	int toInt() const;

	int toTime(int n); 
};