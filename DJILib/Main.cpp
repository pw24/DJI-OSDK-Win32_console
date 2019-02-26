#include <stdlib.h>
#include <iostream>
#include <string>
#include "dji_telemetry.hpp"
#include "dji_vehicle.hpp"		//For creating vehicle object

using namespace std;

class CommsTest
{
public:
	CommsTest();
	~CommsTest();

	//Get API Version number
	char* DLLgetVers(char* test);

	//connect to the platform
	bool Connect(const char* Port, uint32_t baud, bool AdvancedSensing);

	uint32_t getFWVersion();

	bool Activate(uint32_t appID, const char* key);

	bool getTelPacket();

	bool test();


private:
	//Create object for vehicle connection
	Vehicle* vehicle;
};

//default constructor
CommsTest::CommsTest()
{}

bool CommsTest::test()
{
	uint8_t* buffer;
	vehicle->protocolLayer->getDriver()->readall(buffer, 40);

	return 0;
}

//destructor
CommsTest::~CommsTest()
{
	if (vehicle != nullptr)
	{
		delete (vehicle);
		vehicle = nullptr;
	}
}

//Get API Version number
char* CommsTest::DLLgetVers(char* test)
{
	return test;//0.7;
}

//connect to the platform
bool CommsTest::Connect(const char* Port, uint32_t baud, bool AdvancedSensing = false)
{
	bool threadSupport = true;	//disable first so that able to debug
	vehicle = new Vehicle(Port, baud, threadSupport, AdvancedSensing);

	//test if connection works fine
	if (!vehicle->protocolLayer->getDriver()->getDeviceStatus())
	{
		//not working, delete current instances
		delete (vehicle);
		vehicle = nullptr;	//set back to null pointer
		return false;
	}

	return true;
}





//get Firmware version on RPA
uint32_t CommsTest::getFWVersion()
{
	if (vehicle != nullptr)
		return vehicle->getFwVersion();
	else
		return 0;
}

//Activate the RPA for communication
bool CommsTest::Activate(uint32_t appID, const char* key)
{
	if (vehicle != nullptr)
	{
		DJI::OSDK::Vehicle::ActivateData actData;
		actData.ID = appID;
		actData.encKey = new char[65];
		strcpy(actData.encKey, key);
		actData.version = getFWVersion();

		ACK::ErrorCode ack = vehicle->activate(&actData, 1);
		if (ACK::getError(ack))
		{
			ACK::getErrorCodeMessage(ack, __func__);
			delete (vehicle);
			vehicle = nullptr;
			return false;
		}
		else
			return true;
	}
	return false;
}

//function to get a telemetry packet
bool CommsTest::getTelPacket()
{
	bool rtkAvailable = false;	//can only be detected for platforms not compatible with platforms
	ACK::ErrorCode ack = vehicle->broadcast->setBroadcastFreqDefaults(20);

	//for (int i = 0; i < 5; i++)
	//{
		//Sleep(300);
		Telemetry::GlobalPosition telemetry = vehicle->broadcast->getGlobalPosition();
		std::cout << "Lat: " << telemetry.latitude << ", Lng: " << telemetry.longitude << endl;
		cout << endl;
	//}
	

	return false;
}


void main()
{
	cout << "Hello Test" << endl;

	CommsTest* APICalls = new CommsTest();

	APICalls->Connect("COM21", 230400);	//Serial port connection to device

	//=======================
	//insert your platform's application key and ID
	const char* key = "111";
	int ID = 111;
	//=======================

	if (APICalls->Activate(ID, key))
	{
		//only implement if successful
		APICalls->getTelPacket();
	}

	//Sleep(300);
	//APICalls->test();
		

	APICalls->~CommsTest();
	cout << endl << endl;
	system("pause");
}

