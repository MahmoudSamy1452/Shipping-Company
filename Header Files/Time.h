#pragma once
class Time
{
	int Day;
	int Hour;

public:

	Time(int, int);

	void incrementTime();

	bool isTime(Time);
};