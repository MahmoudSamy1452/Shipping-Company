#include "Windows.h"
#include "../Header Files/UI.h"
#include "../Header Files/Company.h"
using namespace std;

void UI::PrintTruck(Truck* t)
{
	if (t->getNoOfCargos() > 0)
	{
		cout << t->getID();
		switch (t->getType())
		{
		case Normal:
			cout << "[";
			t->PrintMovingCargo();
			cout << "]";
			break;
		case Special:
			cout << "(";
			t->PrintMovingCargo();
			cout << ")";
			break;
		case VIP:
			cout << "{";
			t->PrintMovingCargo();
			cout << "}";
			break;
		}
	}
	else
	{
		switch (t->getType())
		{
		case Normal:
			cout << t->getID();
			break;
		case Special:
			cout << t->getID();
			break;
		case VIP:
			cout << t->getID();
			break;
		}
	}
}

ostream& operator<<(ostream& output, Truck*& t)
{
	UI::PrintTruck(t);
	return output;
}

ostream& operator << (ostream& output, Cargo*& c)
{
	output << c->ID;
	return output;
}

UI::UI(Company* company)
{
	this->company = company;
}

void UI::readInterfaceMode()
{
	int mode;
	cout << "Choose Interface: (0) Interactive Mode (1) Step-By-Step Mode  (2) Silent Mode ";
	do
	{
		cin >> mode;
		switch (mode)
		{
		case 0:
			UImode = Interactive;
			break;
		case 1:
			UImode = Step_By_Step;
			break;
		case 2:
			UImode = Silent;
			break;
		default:
			cout << "Invalid Mode. Re-enter a valid mode: ";
		}
	} while (mode < 0 && mode > 2);
}

InterfaceMode UI::getUImode()
{
	return UImode;
}

string UI::readFilename()
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

void UI::PrintBreakLine()
{
	cout << "\n--------------------------------------------------------\n";
}

void UI::wait()
{
	cin.get();
}

void UI::PrintHour()
{
	int LT, ET, MC, ICT, DC, WC;
	cout << endl;
	cout << "Current time (Day:Hour):";
	printTime(company->GetClock());
	cout << endl;
	WC = company->getLengthOfLists(LT, ET, MC, ICT, DC);
	cout << WC << " Waiting Cargos: [";
	company->PrintWaitingNC();
	cout << "] (";
	company->PrintWaitingSC();
	cout << ") {";
	company->PrintWaitingVC();
	cout << "}";
	PrintBreakLine();
	//////////////////////////////////////////////////////////
	cout << LT << " Loading Trucks: ";
	company->PrintLoadingT();
	PrintBreakLine();
	///////////////////////////////////////////////////////////
	cout << ET << " Empty Trucks: ";
	cout << "[";
	company->PrintWaitingNT();
	cout << "], (";
	company->PrintWaitingST();
	cout << "), {";
	company->PrintWaitingVT();
	cout << "}";
	PrintBreakLine();
	///////////////////////////////////////////////////////////
	cout << MC << " Moving Cargos: ";
	company->PrintMovingT();
	PrintBreakLine();
	/////////////////////////////////////////////////////////////
	cout << ICT << " In-Checkup Trucks: ";
	company->PrintTrucksInMaintenance();
	PrintBreakLine();
	/////////////////////////////////////////////////////////////
	cout << DC << " Delivered Cargos: [";
	company->PrintDeliveredNC();
	cout << "] (";
	company->PrintDeliveredSC();
	cout << ") {";
	company->PrintDeliveredVC();
	cout << "}";
	PrintBreakLine();
	if (UImode == Interactive)
		wait();
	else
		Sleep(1000);
}

void UI::StartSilent()
{
	cout << "Silent Mode\n";
	cout << "Simulation Starts...\n";
}