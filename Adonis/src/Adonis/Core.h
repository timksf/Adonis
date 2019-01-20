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

#define ON_EVENT_DECL(event_type) auto on_##event_type(const event_ptr_t<event_type>& event)->void								//straight decl
#define ON_EVENT_DECL_OVERRIDE(event_type) auto on_##event_type(const event_ptr_t<event_type>& event)->void override			//overriding
#define ON_EVENT_DECL_V(event_type) virtual auto on_##event_type(const event_ptr_t<event_type>& event)->void					//virtual
#define ON_EVENT_DECL_P_V(event_type) virtual auto on_##event_type(const event_ptr_t<event_type>& event)->void = 0				//pure virtual
#define ON_EVENT_DECL_V_OVERRIDE(event_type) virtual auto on_##event_type(const event_ptr_t<event_type>& event)->void override	//virtual and overriding


#define BIND_FN(type, fn) std::bind(&type::fn, this, std::placeholders::_1)
#define ON_EVENT(event_type, ...) on_event<event_type>(__VA_ARGS__)
#define ON_EVENT_BIND(event_type, type, fn) ON_EVENT(event_type, BIND_FN(type, fn))

#define ON_EVENT_BIND(event_type, type) ON_EVENT(event_type, BIND_FN(type, on_##event_type))
