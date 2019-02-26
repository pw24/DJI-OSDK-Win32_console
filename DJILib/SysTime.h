#pragma once

#include <Windows.h>

namespace DJI
{
	namespace OSDK
	{
		//Simplistic class to calculate system delay functions (usually utilized standard within Linux Environment)
		class SysTime
		{
		public:
			SysTime();
			~SysTime();
			int static clock_gettime(struct timeval *tv);	//function to get system delay

		private:
			LARGE_INTEGER static getFILETIMEoffset();
		};
	}
}



