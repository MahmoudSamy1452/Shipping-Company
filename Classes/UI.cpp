#include "../Header Files/UI.h"


InterfaceMode UI::readInterfaceMode()
{
	int mode;
	cout << "Choose Interface: (0)Interactive Mode (1)Step-By-Step Mode  (2)Silent Mode";
	while (1)
	{
		cin >> mode;
		switch (mode)
		{
		case 0:
			return Interactive;
		case 1:
			return Step_By_Step;
		case 2:
			return Silent;
		default:
			cout << "Invalid Mode. Re-enter a valid mode: ";
		}
	}
}

string readFilename()
{
	string name;
	cout << "Enter Filename: ";
	cin >> name;
	return name;
}

void UI::printTime(Time t)
{
	cout << t.getDay() << ":" << t.getHour();
}

