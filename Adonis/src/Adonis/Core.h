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

#define ON_EVENT_DECL(event_type)				auto on_##event_type(const Adonis::event_ptr_t<event_type>& event)->void							//straight decl
#define ON_EVENT_DECL_OVERRIDE(event_type)		auto on_##event_type(const Adonis::event_ptr_t<event_type>& event)->void override					//overriding
#define ON_EVENT_DECL_V(event_type)				virtual auto on_##event_type(const Adonis::event_ptr_t<event_type>& event)->void					//virtual
#define ON_EVENT_DECL_P_V(event_type)			virtual auto on_##event_type(const Adonis::event_ptr_t<event_type>& event)->void = 0				//pure virtual
#define ON_EVENT_DECL_V_OVERRIDE(event_type)	virtual auto on_##event_type(const Adonis::event_ptr_t<event_type>& event)->void override			//virtual and overriding
#define BIND_FN(type, fn)						std::bind(&type::fn, this, std::placeholders::_1)
#define ON_EVENT(event_type, ...)				on_event<event_type>(__VA_ARGS__)
#define ON_EVENT_BIND(event_type, type)			ON_EVENT(event_type, BIND_FN(type, on_##event_type))

#define ON_EVENT_BIND_CLIENT(event_type, type)	ON_EVENT(Adonis::event_type, BIND_FN(type, on_##event_type))


#define AD_FALSE 0
#define AD_TRUE 1

#ifdef ADONIS_USE_GLFW
	#define ADONIS_WINDOW_CONTEXT_LIB_NAME	"GLFW"
	#define	ADONIS_KEY_SPACE     		GLFW_KEY_SPACE              
	#define	ADONIS_KEY_APOSTROPHE   	GLFW_KEY_APOSTROPHE         
	#define	ADONIS_KEY_COMMA        	GLFW_KEY_COMMA              
	#define	ADONIS_KEY_MINUS        	GLFW_KEY_MINUS              
	#define	ADONIS_KEY_PERIOD       	GLFW_KEY_PERIOD             
	#define	ADONIS_KEY_SLASH        	GLFW_KEY_SLASH              
	#define	ADONIS_KEY_0            	GLFW_KEY_0                  
	#define	ADONIS_KEY_1            	GLFW_KEY_1                  
	#define	ADONIS_KEY_2            	GLFW_KEY_2                  
	#define	ADONIS_KEY_3            	GLFW_KEY_3                  
	#define	ADONIS_KEY_4            	GLFW_KEY_4                  
	#define	ADONIS_KEY_5            	GLFW_KEY_5                  
	#define	ADONIS_KEY_6            	GLFW_KEY_6                  
	#define	ADONIS_KEY_7            	GLFW_KEY_7                  
	#define	ADONIS_KEY_8            	GLFW_KEY_8                  
	#define	ADONIS_KEY_9            	GLFW_KEY_9                  
	#define	ADONIS_KEY_SEMICOLON    	GLFW_KEY_SEMICOLON          
	#define	ADONIS_KEY_EQUAL        	GLFW_KEY_EQUAL              
	#define	ADONIS_KEY_A            	GLFW_KEY_A                  
	#define	ADONIS_KEY_B            	GLFW_KEY_B                  
	#define	ADONIS_KEY_C            	GLFW_KEY_C                  
	#define	ADONIS_KEY_D            	GLFW_KEY_D                  
	#define	ADONIS_KEY_E            	GLFW_KEY_E                  
	#define	ADONIS_KEY_F            	GLFW_KEY_F                  
	#define	ADONIS_KEY_G            	GLFW_KEY_G                  
	#define	ADONIS_KEY_H            	GLFW_KEY_H                  
	#define	ADONIS_KEY_I            	GLFW_KEY_I                  
	#define	ADONIS_KEY_J            	GLFW_KEY_J                  
	#define	ADONIS_KEY_K            	GLFW_KEY_K                  
	#define	ADONIS_KEY_L            	GLFW_KEY_L                  
	#define	ADONIS_KEY_M            	GLFW_KEY_M                  
	#define	ADONIS_KEY_N            	GLFW_KEY_N                  
	#define	ADONIS_KEY_O            	GLFW_KEY_O                  
	#define	ADONIS_KEY_P            	GLFW_KEY_P                  
	#define	ADONIS_KEY_Q            	GLFW_KEY_Q                  
	#define	ADONIS_KEY_R            	GLFW_KEY_R                  
	#define	ADONIS_KEY_S            	GLFW_KEY_S                  
	#define	ADONIS_KEY_T            	GLFW_KEY_T                  
	#define	ADONIS_KEY_U            	GLFW_KEY_U                  
	#define	ADONIS_KEY_V            	GLFW_KEY_V                  
	#define	ADONIS_KEY_W            	GLFW_KEY_W                  
	#define	ADONIS_KEY_X            	GLFW_KEY_X                  
	#define	ADONIS_KEY_Y            	GLFW_KEY_Y                  
	#define	ADONIS_KEY_Z            	GLFW_KEY_Z                  
	#define	ADONIS_KEY_LEFT_BRACKET 	GLFW_KEY_LEFT_BRACKET       
	#define	ADONIS_KEY_BACKSLASH    	GLFW_KEY_BACKSLASH          
	#define	ADONIS_KEY_RIGHT_BRACKET	GLFW_KEY_RIGHT_BRACKET      
	#define	ADONIS_KEY_GRAVE_ACCENT 	GLFW_KEY_GRAVE_ACCENT       
	#define	ADONIS_KEY_WORLD_1      	GLFW_KEY_WORLD_1            
	#define	ADONIS_KEY_WORLD_2      	GLFW_KEY_WORLD_2            
	#define	ADONIS_KEY_ESCAPE       	GLFW_KEY_ESCAPE             
	#define	ADONIS_KEY_ENTER        	GLFW_KEY_ENTER              
	#define	ADONIS_KEY_TAB          	GLFW_KEY_TAB                
	#define	ADONIS_KEY_BACKSPACE    	GLFW_KEY_BACKSPACE          
	#define	ADONIS_KEY_INSERT       	GLFW_KEY_INSERT             
	#define	ADONIS_KEY_DELETE       	GLFW_KEY_DELETE             
	#define	ADONIS_KEY_RIGHT        	GLFW_KEY_RIGHT              
	#define	ADONIS_KEY_LEFT         	GLFW_KEY_LEFT               
	#define	ADONIS_KEY_DOWN         	GLFW_KEY_DOWN               
	#define	ADONIS_KEY_UP           	GLFW_KEY_UP                 
	#define	ADONIS_KEY_PAGE_UP      	GLFW_KEY_PAGE_UP            
	#define	ADONIS_KEY_PAGE_DOWN    	GLFW_KEY_PAGE_DOWN          
	#define	ADONIS_KEY_HOME         	GLFW_KEY_HOME               
	#define	ADONIS_KEY_END          	GLFW_KEY_END                
	#define	ADONIS_KEY_CAPS_LOCK    	GLFW_KEY_CAPS_LOCK          
	#define	ADONIS_KEY_SCROLL_LOCK  	GLFW_KEY_SCROLL_LOCK        
	#define	ADONIS_KEY_NUM_LOCK     	GLFW_KEY_NUM_LOCK           
	#define	ADONIS_KEY_PRINT_SCREEN 	GLFW_KEY_PRINT_SCREEN       
	#define	ADONIS_KEY_PAUSE        	GLFW_KEY_PAUSE              
	#define	ADONIS_KEY_F1           	GLFW_KEY_F1                 
	#define	ADONIS_KEY_F2           	GLFW_KEY_F2                 
	#define	ADONIS_KEY_F3           	GLFW_KEY_F3                 
	#define	ADONIS_KEY_F4           	GLFW_KEY_F4                 
	#define	ADONIS_KEY_F5           	GLFW_KEY_F5                 
	#define	ADONIS_KEY_F6           	GLFW_KEY_F6                 
	#define	ADONIS_KEY_F7           	GLFW_KEY_F7                 
	#define	ADONIS_KEY_F8           	GLFW_KEY_F8                 
	#define	ADONIS_KEY_F9           	GLFW_KEY_F9                 
	#define	ADONIS_KEY_F10          	GLFW_KEY_F10                
	#define	ADONIS_KEY_F11          	GLFW_KEY_F11                
	#define	ADONIS_KEY_F12          	GLFW_KEY_F12                
	#define	ADONIS_KEY_F13          	GLFW_KEY_F13                
	#define	ADONIS_KEY_F14          	GLFW_KEY_F14                
	#define	ADONIS_KEY_F15          	GLFW_KEY_F15                
	#define	ADONIS_KEY_F16          	GLFW_KEY_F16                
	#define	ADONIS_KEY_F17          	GLFW_KEY_F17                
	#define	ADONIS_KEY_F18          	GLFW_KEY_F18                
	#define	ADONIS_KEY_F19          	GLFW_KEY_F19                
	#define	ADONIS_KEY_F20          	GLFW_KEY_F20                
	#define	ADONIS_KEY_F21          	GLFW_KEY_F21                
	#define	ADONIS_KEY_F22          	GLFW_KEY_F22                
	#define	ADONIS_KEY_F23          	GLFW_KEY_F23                
	#define	ADONIS_KEY_F24          	GLFW_KEY_F24                
	#define	ADONIS_KEY_F25          	GLFW_KEY_F25                
	#define	ADONIS_KEY_KP_0         	GLFW_KEY_KP_0               
	#define	ADONIS_KEY_KP_1         	GLFW_KEY_KP_1               
	#define	ADONIS_KEY_KP_2         	GLFW_KEY_KP_2               
	#define	ADONIS_KEY_KP_3         	GLFW_KEY_KP_3               
	#define	ADONIS_KEY_KP_4         	GLFW_KEY_KP_4               
	#define	ADONIS_KEY_KP_5         	GLFW_KEY_KP_5               
	#define	ADONIS_KEY_KP_6         	GLFW_KEY_KP_6               
	#define	ADONIS_KEY_KP_7         	GLFW_KEY_KP_7               
	#define	ADONIS_KEY_KP_8         	GLFW_KEY_KP_8               
	#define	ADONIS_KEY_KP_9         	GLFW_KEY_KP_9               
	#define	ADONIS_KEY_KP_DECIMAL   	GLFW_KEY_KP_DECIMAL         
	#define	ADONIS_KEY_KP_DIVIDE    	GLFW_KEY_KP_DIVIDE          
	#define	ADONIS_KEY_KP_MULTIPLY  	GLFW_KEY_KP_MULTIPLY        
	#define	ADONIS_KEY_KP_SUBTRACT  	GLFW_KEY_KP_SUBTRACT        
	#define	ADONIS_KEY_KP_ADD       	GLFW_KEY_KP_ADD             
	#define	ADONIS_KEY_KP_ENTER     	GLFW_KEY_KP_ENTER           
	#define	ADONIS_KEY_KP_EQUAL     	GLFW_KEY_KP_EQUAL           
	#define	ADONIS_KEY_LEFT_SHIFT   	GLFW_KEY_LEFT_SHIFT         
	#define	ADONIS_KEY_LEFT_CONTROL 	GLFW_KEY_LEFT_CONTROL       
	#define	ADONIS_KEY_LEFT_ALT     	GLFW_KEY_LEFT_ALT           
	#define	ADONIS_KEY_LEFT_SUPER   	GLFW_KEY_LEFT_SUPER         
	#define	ADONIS_KEY_RIGHT_SHIFT  	GLFW_KEY_RIGHT_SHIFT        
	#define	ADONIS_KEY_RIGHT_CONTROL	GLFW_KEY_RIGHT_CONTROL      
	#define	ADONIS_KEY_RIGHT_ALT    	GLFW_KEY_RIGHT_ALT          
	#define	ADONIS_KEY_RIGHT_SUPER  	GLFW_KEY_RIGHT_SUPER        
	#define	ADONIS_KEY_MENU         	GLFW_KEY_MENU               
	#define	ADONIS_KEY_LAST    			GLFW_KEY_LAST    
#else
	#ifdef ADONIS_USE_SDL2
		#define ADONIS_WINDOW_CONTEXT_LIB "SDL2"
	//TODO
	#endif
#endif