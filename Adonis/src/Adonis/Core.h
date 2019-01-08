#pragma once
#pragma warning( disable: 4251 )

#ifdef ADONIS_PLATFORM_WINDOWS
	#ifdef ADONIS_BUILD_DLL
		#define ADONIS_API __declspec(dllexport)
	#else
		#define ADONIS_API __declspec(dllimport)
	#endif
#else
	#error Only windows is supported at the time
#endif
