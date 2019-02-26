/*! @file linux_serial_device.hpp
 *  @version 3.3
 *  @date Jun 15 2017
 *
 *  @brief
 *  Serial device hardware implementation for Windows machines.
 *
 *  @details
 *  This is a generic serial device implementation.
 *
 *  The Vehicle class handles Serial drivers, so you do not need to
 *  instantiate drivers in your code.
 *
 */

#ifndef WINSERIALDEVICE_H
#define WINSERIALDEVICE_H

#include <Windows.h>
#include <cstring>
#include "dji_hard_driver.hpp"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <stdlib.h>
#include "SysTime.h"

namespace DJI
{

namespace OSDK
{
	//class for implementing UART connection for API
	class Win_serial_device : public HardDriver
	{
	
	public:
		static const int BUFFER_SIZE = 2048;

		Win_serial_device(const char* device, uint32_t baudrate);	//Constructor
		~Win_serial_device();										//Deconstructor

		void init();							//Upen the serial connection
		bool getDeviceStatus();					//If device connected or not
		time_ms getTimeStamp();					//Return a timestamp from connection if required

		void setBaudrate(uint32_t baudrate);	//set baudrate value
		void setDevice(const char* device);		//set Port identification name
		void Close();							//close the connection / device

		int checkBaudRate(uint8_t(&buf)[BUFFER_SIZE]);

		//still implement these functions
		//===============================
		int setSerialPureTimedRead();
		int unsetSerialPureTimedRead();
		//===============================

		
		size_t send(const uint8_t* buf, size_t len);		//transmission of byte stream of connection
		int serialRead(uint8_t* buf, int len);				//read fixes byte length of data from connection
		size_t readall(uint8_t* buf, size_t maxlen);		//reception of all bytes in stream from connection

	private:

		const char* m_device;		//The name of serial port
		uint32_t m_baudrate;		//The baudrate of the serial port
		HANDLE m_serialHandle;		//Serial Port object to manipulate
		bool deviceStatus;			//identification if device is connected or not
		bool serialConfig(int baudrate, char data_bits, char parity_bits, char stop_bits, bool testForData = false);	
		bool _PortTimeouts();		//set port timeouts to during config setup
		bool _PortParams(int baudrate, char data_bits, char parity_bits, char stop_bits);	//set basic port config parameters	
		bool _PortReceive();		//Activate port reception on UART port
	};
}

}




#endif