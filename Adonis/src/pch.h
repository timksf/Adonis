#pragma once

#include <memory>
#include <iostream>

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

//
#include "Adonis/Log.h"
//

#ifdef ADONIS_PLATFORM_WINDOWS
	#include <Windows.h>

	#ifdef ADONIS_GLLOADER_GLAD
		#include <glad/glad.h>
	#else
		#error Only glad supported yet
	#endif //ADONIS_GLLOADER_GLAD

#endif

#ifdef ADONIS_USE_GLFW
	#include <GLFW/glfw3.h>
#else
	#error Only supported window/context library is glfw
#endif //ADONIS_USE_GLFW
