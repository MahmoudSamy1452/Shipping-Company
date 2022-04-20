#include "../Header Files/UI.h"

ostream& operator<<(ostream& output, const Truck& t)
{
	output << t.getID() << ", ";
	return output;
}
ostream& operator<<(ostream& output, const Cargo& c)
{
	output << c.getID() << ", ";
	return output;
}

void UI::readInterfaceMode()
{
	int mode;
	cout << "Choose Interface: (0)Interactive Mode (1)Step-By-Step Mode  (2)Silent Mode";
	while (1)
	{
		cin >> mode;
		switch (mode)
		{
		case 0:
			 UImode =Interactive;
		case 1:
			 UImode =Step_By_Step;
		case 2:
			 UImode = Silent;
		default:
			cout << "Invalid Mode. Re-enter a valid mode: ";
		}
	}
}

InterfaceMode UI::getUImode()
{
	return UImode;
}

string UI::readFilename()
{
	string name = "";
	cout << "Enter Filename: ";
	cin >> name;
	return name;
}

void UI::printTime(Time t)
{
	cout << t.getDay() << ":" << t.getHour();
}

void UI::PrintString(const string& str)
{
	cout << str;
}

void UI::PrintBreakLine()
{
	cout << "\n--------------------------------------------------------\n";
}

void UI::wait()
{
	cin.get();
}