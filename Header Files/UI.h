#pragma once
#include <iostream>
using namespace std;

#include "../DEFS.h"
#include "Time.h"

class UI
{
public:
	InterfaceMode readInterfaceMode();
	string readFilename();
	void printTime(Time t);
};