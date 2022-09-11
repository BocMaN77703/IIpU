#include <iostream>
#include <Windows.h>

enum AC_Status {
	Offline = 0,
	Online = 1,
	StatusUnknown = 255
};
enum Battery_FLAG {
	HighCapacity = 1,
	LowCapacity = 2,
	CriticalLowCapacity = 4,
	HighCapacityCharging = 9,
	LowCapacityCharging = 10,
	CriticalLowCapacityCharging = 12,
	NoSystemBattery = 128,
	StatUnknown = 255
};

class Battery
{
public: 
	Battery();
	~Battery();
	void SetInfo();
	void GetInfo();
	void printBatteryChemistry();
private:
	AC_Status ac;
	Battery_FLAG bf;
	unsigned int life_percent;
	bool power_saving_status;
	DWORD BatteryLifeTime;
};

