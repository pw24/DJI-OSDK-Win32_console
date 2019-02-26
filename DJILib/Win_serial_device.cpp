/*! @file Win_serial_device.cpp
 *  @version 0.6
 *  @date Feb 07 2019
 *
 *  @brief
 *  Serial device hardware implementation for windows machines.
 *  This is a generic Windows serial device implementation.
 *
 *  Use this in your own Windows-based DJI Onboard SDK implementations.
 *
 */

#include "Win_serial_device.h"

using namespace DJI::OSDK;

//Class constructor to setup Serial port
Win_serial_device::Win_serial_device(const char* Device, uint32_t Baudrate)
{
	m_device = Device;
	m_baudrate = Baudrate;
	deviceStatus = false;
}

//Class destructor
Win_serial_device::~Win_serial_device()
{
	this->Close();
}

//set baud rate for communication
void Win_serial_device::setBaudrate(uint32_t baudrate)
{
	m_baudrate = baudrate;
}

//set COM port name for communication
void Win_serial_device::setDevice(const char* device)
{
	//set Com port name
	m_device = device;
}

//Initiate port for communication
void Win_serial_device::init()
{
	DSTATUS("Attempting to open device %s with baudrate %u...\n", m_device,
		m_baudrate);

	const char* slashes = "\\\\.\\";
	char* Port = (char*)malloc((strlen(slashes) + strlen(m_device)) * sizeof(char));
	strcpy(Port, slashes);
	strcat(Port, m_device);

	//close any existing connections
	if (deviceStatus)
		this->Close();

	m_serialHandle = CreateFile(Port,            //port name 
		GENERIC_READ | GENERIC_WRITE, //Read/Write   				 
		0,            // No Sharing                               
		NULL,         // No Security                              
		OPEN_EXISTING,// Open existing port only                     
		0,            // Non Overlapped I/O                           
		NULL);        // Null for Comm Devices

	if (m_serialHandle == INVALID_HANDLE_VALUE)
	{
		this->Close();
		deviceStatus = false;
		DERROR("...Failed to start serial\n");
	}
	else
	{
		DSTATUS("...Serial started successfully.\n");
		deviceStatus = this->serialConfig(m_baudrate, 8, 'N', 1);	//config the port according to specified baud
	}


}

//config the serial port according to specifications
bool Win_serial_device::serialConfig(int baudrate, char data_bits, char parity_bits, char stop_bits, bool testForData)
{
	deviceStatus = false;

	if (_PortParams(baudrate, data_bits, parity_bits, stop_bits))	//set Port's basic parameters
		if (_PortTimeouts())		//set Port's timeout parameters
			if (_PortReceive())			//enable Port reception commands
				deviceStatus = true;

	DSTATUS("... Serial Config done, Successful: " + deviceStatus);
	return deviceStatus;
}

//Activate port reception on UART port
bool Win_serial_device::_PortReceive()
{
	bool status = SetCommMask(m_serialHandle, EV_RXCHAR);	//activate data reception over port
	if (status)
	{
		DSTATUS("Port data reception enabled");
		//DWORD dwEventMask;     // Event mask to trigger
		//WaitCommEvent(m_serialHandle, &dwEventMask, NULL); //Wait for the character to be received
	}		
	else
	{
		DERROR("Port data reception could not be enabled");
	}

	return status;
}

//set Timeout settings for port
bool Win_serial_device::_PortTimeouts()
{
	//Time periods in ms, how long is waited before exiting a comm read / write function
	//https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-_commtimeouts

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	bool status = SetCommTimeouts(m_serialHandle, &timeouts);	//set timeout limits for reception
	if (status)
		DSTATUS("Port timeout parameters successfully set");
	else
		DERROR("Could not config port timeouts");

	return status;
}

//set basic port config parameters
bool Win_serial_device::_PortParams(int baudrate, char data_bits, char parity_bits, char stop_bits)
{
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	deviceStatus = GetCommState(m_serialHandle, &dcbSerialParams);	//get current settings of port

	//set size of byte
	if ((data_bits == 7) | (data_bits == 8))
		dcbSerialParams.ByteSize = data_bits;	//set size of byte -> which will be 7 / 8
	else
		dcbSerialParams.ByteSize = 8;

	//set parity of bits
	switch (parity_bits)
	{
		/* odd */
	case 'O':
	case 'o':
		dcbSerialParams.Parity = ODDPARITY;
		break;
		/* even */
	case 'E':
	case 'e':
		dcbSerialParams.Parity = EVENPARITY;
		break;
		/* none */
	case 'N':
	case 'n':
		dcbSerialParams.Parity = NOPARITY;
		break;
	default:
		dcbSerialParams.Parity = NOPARITY;
		break; //8N1 default config
	}

	//configure baudrate
	dcbSerialParams.BaudRate = baudrate;
	/*int st_baud[] = { CBR_4800,  CBR_9600, CBR_19200, CBR_38400,
					CBR_57600, CBR_115200, CBR_256000 };
	int std_rate[] = { 4800,   9600,   19200,   38400,   57600,  115200, 256000 };
	for (int i = 0; i < sizeof(st_baud); i++)
		if (std_rate[i] == baudrate)
		{
			dcbSerialParams.BaudRate = st_baud[i];
			break;
		}*/

	//configure stop bits
	//dcbSerialParams.StopBits = stop_bits;
	switch (stop_bits)
	{
	case 1:
		dcbSerialParams.StopBits = ONESTOPBIT;
		break;
	case 2:
		dcbSerialParams.StopBits = TWOSTOPBITS;
	default:
		dcbSerialParams.StopBits = ONESTOPBIT;
		break;
	}

	bool status = SetCommState(m_serialHandle, &dcbSerialParams);	//set port parameters
	if(!status)
		DERROR("failed to activate serial configuration\n");
	else
		DSTATUS("Port parameters successfully set");

	return status;
}

//this function currently not implememented in config function
//to set min characters to check on line connection -> just to determine if line is open and active
int Win_serial_device::setSerialPureTimedRead()
{
	//not implemented, therefore, doesn't matter if last parameter set true or false
	return this->serialConfig(m_baudrate, 8, 'N', 1, true);
}

//this function currently doesn't have any purpose...
//but purpose to unset min characters to check on line connection
int Win_serial_device::unsetSerialPureTimedRead()
{
	return this->serialConfig(m_baudrate, 8, 'N', 1, false);
}

//return the device connection status from Serial port
bool Win_serial_device::getDeviceStatus()
{
	return deviceStatus;
}

//get current time stamp from connection
time_ms Win_serial_device::getTimeStamp()
{
	return (uint32_t)time(NULL);
}

//try to determine the baud rate at which communication is done on the line
int Win_serial_device::checkBaudRate(uint8_t(&buf)[BUFFER_SIZE]) 
{
	int lengthForCheck = 200;
	int timeoutInSeconds = 2;

	struct timeval curTime, absTimeout;
	// Use clock_gettime instead of getttimeofday for compatibility with POSIX
	// APIs
    SysTime::clock_gettime(&curTime);
	absTimeout.tv_sec = curTime.tv_sec + timeoutInSeconds;

	int receivedBytes = this->serialRead(buf, lengthForCheck);

	while (curTime.tv_sec < absTimeout.tv_sec)
	{
		if (receivedBytes < lengthForCheck)
			receivedBytes +=
			this->serialRead(buf + receivedBytes, lengthForCheck - receivedBytes);
		else
			break;

		SysTime::clock_gettime(&curTime);
	}
	if (curTime.tv_sec >= absTimeout.tv_sec)
		return -1;
	if (buf + BUFFER_SIZE == std::find(buf, buf + BUFFER_SIZE, 0xAA))
		return -2;

	return 1;
}

//send byte stream over the connection
size_t Win_serial_device::send(const uint8_t* buf, size_t len)
{
	DWORD  dNoOFBytestoWrite = len;              // No of bytes to write into the port
	DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port
	//dNoOFBytestoWrite = sizeof(buf); // Calculating the no of bytes to write into the port

	bool status = WriteFile(m_serialHandle,               // Handle to the Serialport
		buf,				// Data to be written to the port 
		dNoOFBytestoWrite,   // No of bytes to write into the port
		&dNoOfBytesWritten,  // No of bytes written to the port
		NULL);

	return dNoOfBytesWritten;	//return the number of bytes written over connection
}

//Read all the bytes contained on the connection stream
size_t Win_serial_device::readall(uint8_t* buf, size_t maxlen)
{
	return this->serialRead(buf, maxlen);
}

//read a number of bytes available on the connection stream
//! Current _serialRead behavior: Wait for 500 ms between characters till 18
//! char, read 18 characters if data available & return
//! 500 ms: long timeout to make sure that if we query the input buffer in the
//! middle of a packet we still get the full packet
//! 18 char: len of most ACK packets.
int Win_serial_device::serialRead(uint8_t* buf, int len)
{
	int ret = -1;
	if (NULL == buf)
	{
		return -1;
	}
	else
	{
		DWORD noBytesRead;
		bool success = ReadFile(m_serialHandle, &buf, len, &noBytesRead, NULL);
		return noBytesRead;	//number of bytes read
	}
}

//Write functionality to close port
void Win_serial_device::Close()
{
	if (m_serialHandle == INVALID_HANDLE_VALUE)
	{
		DSTATUS("Port not picked up as open, so that can close");
		deviceStatus = false;
	}	
	else
	{
		if (CloseHandle(m_serialHandle))
		{
			DSTATUS("...Serial port close successful");
			deviceStatus = false;
		}
			
		else
			DERROR("...Serial port close unsuccessful");
	}

}
