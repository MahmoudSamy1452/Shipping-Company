#pragma once
#include <iostream>
#include "../DEFS.h"

using namespace std;

class Company;
class Time;
class Truck;

class UI
{
	InterfaceMode UImode;
	Company* company;
public:
	UI(Company*);
	void readInterfaceMode();
	InterfaceMode getUImode();
	string readFilename();
	void PrintErrorMessage();
	void printTime(Time t);
	void PrintBreakLine();
	void PrintHour();
	void wait();
	static void PrintTruck(Truck* t);
	void StartSilent();
};