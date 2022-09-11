#include <iostream>
#include <Windows.h>
#include "Battery.h"
#include <conio.h>
#include <powrprof.h>
#pragma comment(lib, "user32")


using namespace std;

char menu()
{
	cout << "1.Sleep\n2.Hibernate\n0.Exit\nPress any other key to show battery info\n";
	char choice;
	keybd_event(0x08, 0, 0, 0);
	keybd_event(0x08, 0, KEYEVENTF_KEYUP, 0);
	cin >> choice;
	return choice;
}

int main() {
	

	Battery battery;
	while (1)
	{
		if (_kbhit())
		{
			rewind(stdin);
			system("cls");
			
			switch (menu())
			{
			case '1':
				SetSuspendState(FALSE, FALSE, FALSE);
				break;
			case '2':
				SetSuspendState(TRUE, FALSE, FALSE);
				break;
			case '0':
				return 0;
				break;
			default:
				system("cls");
				break;
			}
		}

		battery.SetInfo();
		battery.GetInfo();
		Sleep(1000);
		system("cls");
	}


}