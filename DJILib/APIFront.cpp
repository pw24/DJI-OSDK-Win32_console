#include "APIFront.h";

using namespace std;

//Get API Version number
char* __stdcall DLLgetVers(char* test)
{
	return test;//0.7;
}

//connect to the platform
bool __stdcall Connect(char* Port, uint32_t baud, bool AdvancedSensing)
{
	bool threadSupport = true;
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
uint32_t __stdcall getFWVersion()
{
	if (vehicle != nullptr)
		return vehicle->getFwVersion();
	else
		return 0;
}

//Activate the RPA for communication
bool __stdcall Activate(uint32_t appID, char* key)
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



