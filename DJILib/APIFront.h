#pragma once
#include <string>
#include "dji_telemetry.hpp"
#include "dji_vehicle.hpp"		//For creating vehicle object


using namespace std;

//Create object for vehicle connection
Vehicle* vehicle;

extern "C"
{
	__declspec(dllexport) char* __stdcall DLLgetVers(char*);

	__declspec(dllexport) bool __stdcall Connect(char*, uint32_t, bool);

	__declspec(dllexport) bool __stdcall Activate(uint32_t, char*);

	__declspec(dllexport) uint32_t __stdcall getFWVersion();
}


