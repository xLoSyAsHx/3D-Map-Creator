#ifndef PROGRAM_DEFINES_H
#define PROGRAM_DEFINES_H

#include <iostream>
#include <chrono>

#define chrono_cast_mics(start, end) std::chrono::duration_cast<std::chrono::microseconds> (end - start)
#define chrono_cast_mils(start, end) std::chrono::duration_cast<std::chrono::microseconds> (end - start)

#define chrono_now std::chrono::system_clock::now()

namespace PD {
	enum 
	{
		WM_CHECKVERTDATA,
		WM_CHECHOBJDATA,
		WM_CHANGECURSOR
	};
}

#endif PROGRAM_DEFINES_H
