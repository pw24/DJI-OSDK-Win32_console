# DJI-OSDK-Win32

The purpose of this repo is to convert the DJI OSDK (c++) to a working API from Windows environment. Currently there are STM, Linux and Qt extractions, but not for c# from Windows environment. Original [DJI OSDK repo](https://github.com/dji-sdk/Onboard-SDK).
So the goal is: 

1. First to make the DJI OSDK implementation compatible for execution from Windows environment (by adapting the Linux OSDK application accordingly)
2. To distinguish relevant functions from the implementation for exportation to dynamic linked library (DLL)
3. To load and implement the DLL from within .Net (c#) environment / applications.
4. Merge / produce the repository back into the main DJI OSDK repository 

Repository entail runnable console application from Visual Studio, connected to DJI M210 RPA platform.

## Current Status

Windows / Linux environment dependencies have been addressed by:

1. Wrinting a c++ Win32 UART connection class and inserted into the DJI API set of classes "Win_serial_device" .cpp & .h
2. Including dependencies to Pthread for execution in Win environment
3. Included "#if _WIN32" implementation in classes such as threading manager, for proper initialization in Win environment. 
4. Addressed all further _minor_ errors for compiling the API into a console application (for debugging purposes until fully functional.

At this point the application executes, but no UART connection reply is received after, implementing _vehicle->activate_, with the following output:

> Hello Test
>
> STATUS/1 @ DJI::OSDK::Win_serial_device::init, L47: Attempting to open device COM21 with baudrate 230400...
>
> STATUS/1 @ DJI::OSDK::Win_serial_device::init, L75: ...Serial started successfully.
>
> STATUS/1 @ DJI::OSDK::Win_serial_device::_PortParams, L202: Port parameters successfully set
> STATUS/1 @ DJI::OSDK::Win_serial_device::_PortTimeouts, L129: Port timeout parameters successfully set
> STATUS/1 @ DJI::OSDK::Win_serial_device::_PortReceive, L102: Port data reception enabled
> STATUS/1 @ DJI::OSDK::Win_serial_device::serialConfig, L92: .. Serial Config done, Successful:
> STATUS/1 @ DJI::OSDK::Win_serial_device::_PortParams, L202: Port parameters successfully set
> STATUS/1 @ DJI::OSDK::Win_serial_device::_PortTimeouts, L129: Port timeout parameters successfully set
> STATUS/1 @ DJI::OSDK::Win_serial_device::_PortReceive, L102: Port data reception enabled
> STATUS/1 @ DJI::OSDK::Win_serial_device::serialConfig, L92: .. Serial Config done, Successful:
> ERRORLOG/1 @ getDroneVersion, L1378: Drone version not obtained! Please do not proceed.
> Possible reasons:
>         Serial port connection:
>                * SDK is not enabled, please check DJI Assistant2 -> SDK -> [v] Enable API Control.
>                * Baudrate is not correct, please double-check from DJI Assistant2 -> SDK -> baudrate.
>                * TX and RX pins are inverted.
>                * Serial port is occupied by another program.
>                * Permission required. Please do 'sudo usermod -a -G dialout $USER' (you do not need to replace $USER with your  username). Then logout and login again
>
> ERRORLOG/1 @ activate, L1267: Unable to initialize some vehicle components!
> STATUS/1 @ getErrorCodeMessage, L656: Activate
> STATUS/1 @ getCommonErrorCodeMessage, L722: ACK_NO_RESPONSE_ERROR
> 
> STATUS/1 @ DJI::OSDK::Win_serial_device::Close, L321: ...Serial port close successful
>
> Press any key to continue . . .

## Contribute

Anyone with the same goals, to start implementing the DJI OSDK from .Net environment, is welcome to branch and help.
