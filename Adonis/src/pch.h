#pragma once

#include <memory>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <sstream>
#include <string_view>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <list>
#include <functional>
#include <type_traits>
#include <cmath>
#include "nlohmann/json.hpp"


//
#include "Adonis/Log.h"
//

#ifndef ADONIS_WINDOW_CONTEXT_LIB
	#ifndef ADONIS_USE_GLFW
		#define ADONIS_USE_GLFW //Default lib
	#endif
#else 
	#if ADONIS_WINDOW_CONTEXT_LIB == 0
		#define ADONIS_USE_GLFW
	#else
		#error Define of ADONIS_WINDOW_CONTEXT_LIB did not match any of the supported values, supported values are: 0 (GLFW)
	#endif

#endif

#ifdef ADONIS_PLATFORM_WINDOWS
	#include <Windows.h>

	#ifndef ADONIS_GLLOADER
		#ifndef ADONIS_GLLOADER_GLAD
				#define ADONIS_GLLOADER_GLAD //default loader
		#endif
	#else
		#if ADONIS_GLLOADER == 0
			#define ADONIS_GLLOADER_GLAD
		#else
			#error Define of ADONIS_GLLOADER did not match any of the supported values, supported values are: 0 (GLAD)
		#endif
	#endif

	#ifdef ADONIS_GLLOADER_GLAD
		#include <glad/glad.h>
	#else
		#error Please define ADONIS_GLLOADER_GLAD
	#endif 

#endif

#ifdef ADONIS_USE_GLFW
	#include <GLFW/glfw3.h>
#else
	#error Please define ADONIS_USE_GLFW
#endif 

#define ADONIS_USE_OPENGL
