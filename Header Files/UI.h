#pragma once
#include <iostream>
using namespace std;

#include "../DEFS.h"
#include "Time.h"
#include "Truck.h"

class UI
{
	InterfaceMode UImode;
public:
	void readInterfaceMode();
	InterfaceMode getUImode();
	string readFilename();
	void printTime(Time t);
	void PrintString(const string& str);
	void PrintBreakLine();
	void wait();
};