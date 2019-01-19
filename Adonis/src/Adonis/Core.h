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


#ifdef ADONIS_PLATFORM_WINDOWS
	#define AD_CLIENT_ASSERT(x, ...)\
								if(!x){\
									AD_CLIENT_ERROR("Assertion failed: {0}", __VA_ARGS__);\
									__debugbreak();\
								}
	#define AD_CORE_ASSERT(x, ...)\
								if(!x){\
									AD_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__);\
									__debugbreak();\
								}
#endif //ADONIS_PLATFORM_WINDOWS

#define AD_NAME_FROM_TYPE(type) std::string(#type)

#define AD_C_STYLE_ARRAYSIZE(x) (static_cast<size_t>((sizeof(x) / sizeof(*x))))
#define AD_ARRAYSIZE(x) AD_C_STYLE_ARRAYSIZE(x)