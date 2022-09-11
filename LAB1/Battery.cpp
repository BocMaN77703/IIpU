#include "Battery.h"

#pragma comment(lib, "PowrProf.lib")
#pragma comment (lib, "setupapi.lib")
#include <Windows.h>
#include <iostream>
#include <powrprof.h>
#include <thread>   
#include "conio.h"
#include <Poclass.h>
#include <Setupapi.h>
#include <devguid.h>

using namespace std;

Battery::Battery() 
{
	SetInfo();
}
Battery::~Battery() {}

void Battery::printBatteryChemistry() {
	HDEVINFO DeviceInfoSet;
	DeviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	SP_DEVICE_INTERFACE_DATA DeviceInterfaceData = { 0 };
	ZeroMemory(&DeviceInterfaceData, sizeof(SP_DEVINFO_DATA));
	DeviceInterfaceData.cbSize = sizeof(SP_DEVINFO_DATA);

	SetupDiEnumDeviceInterfaces(DeviceInfoSet, NULL, &GUID_DEVCLASS_BATTERY, 0, &DeviceInterfaceData);
	DWORD cbRequired = 0;

	SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, NULL, NULL, &cbRequired, NULL);
	PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, cbRequired);
	pdidd->cbSize = sizeof(*pdidd);

	SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &DeviceInterfaceData, pdidd, cbRequired, &cbRequired, NULL);
	HANDLE hBattery = CreateFile(pdidd->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	BATTERY_QUERY_INFORMATION BatteryQueryInformation = { 0 };
	DWORD bytesWait = 0;
	DWORD bytesReturned = 0;
	DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_TAG, &bytesWait, sizeof(bytesWait), &BatteryQueryInformation.BatteryTag,
		sizeof(BatteryQueryInformation.BatteryTag), &bytesReturned, NULL) && BatteryQueryInformation.BatteryTag;

	BATTERY_INFORMATION BatteryInfo = { 0 };
	BatteryQueryInformation.InformationLevel = BatteryInformation;

	DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_INFORMATION, &BatteryQueryInformation, sizeof(BatteryQueryInformation),
		&BatteryInfo, sizeof(BatteryInfo), &bytesReturned, NULL);

	for (int b = 0; b < 4; b++)
	{
		std::cout << BatteryInfo.Chemistry[b];
	};

	LocalFree(pdidd);
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);
}

void Battery::SetInfo() {

	SYSTEM_POWER_STATUS descriptor;
	GetSystemPowerStatus(&descriptor);

	this->ac = (AC_Status)descriptor.ACLineStatus;
	this->bf = (Battery_FLAG)descriptor.BatteryFlag;
	this->life_percent = (unsigned int)descriptor.BatteryLifePercent;
	this->power_saving_status = (bool)descriptor.SystemStatusFlag;
	this->BatteryLifeTime = descriptor.BatteryLifeTime;

}

void Battery::GetInfo()
{
	std::cout << "Battery INFO :\n" << "AC STATUS : ";
	switch (this->ac)
	{
	case Offline:
		std::cout << "Offline\n";
		break;

	case Online:
		std::cout << "Online\n";
		break;
	case StatusUnknown:
		std::cout << "Can`t read INFO\n";
	}
	std::cout << "Battery state of charge : ";
	switch (this->bf)
	{
	case HighCapacity:
		std::cout << "High level\n";
		break;
	case LowCapacity:
		std::cout << "Low level\n";
		break;
	case CriticalLowCapacity:
		std::cout << "Critical level\n";
		break;
	case HighCapacityCharging:
		std::cout << "High Capacity (Charging)\n";
		break;
	case CriticalLowCapacityCharging:
		std::cout << "Critical Low Capacity (Charging)\n";
		break;
	case LowCapacityCharging:
		std::cout << "Low Capacity (Charging)\n";
		break;
	case NoSystemBattery:
		std::cout << "No system battery...\n";
		break;
	case StatUnknown:
		std::cout << "Can`t read INFO\n";
	}
	std::cout << "Battery status % : ";
	switch (this->life_percent)
	{
	default:
		std::cout << life_percent << "%\n";
		break;
	case 255:
		std::cout << "Can`t read INFO\n";
	}
	std::cout << "Power saving status : ";
	switch (this->power_saving_status) {
	case false:
		std::cout << "Battery saver is disabled\n";
		break;
	case true:
		std::cout << "Battery saver is enabled\n";
	}
	cout << "Battery type : ";
	printBatteryChemistry();
	cout << endl;
	if (this->ac == Online)
	{
		cout << "Battery life time : Charging"<<endl;
	}
	else
	{
		cout << "Battery life time : " << this->BatteryLifeTime / 3600 << ":";
		cout << (this->BatteryLifeTime % 3600) / 60 << ":";
		cout << (this->BatteryLifeTime % 3600) % 60 << endl;
	}
}
