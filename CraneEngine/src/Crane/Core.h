#pragma once

#ifdef CR_PLATFORM_WINDOWS
	#ifdef CR_BUILD_DLL
		#define CRANE_API __declspec(dllexport)
	#else
		#define CRANE_API __declspec(dllimport)
	#endif
#else 
	#error Crane only supports windows
#endif // CR_PLATFORM_WINDOWS