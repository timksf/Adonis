workspace "Adonis"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Adonis/vendor/GLFW/include"
IncludeDir["GLAD"] = "Adonis/vendor/glad/include"
IncludeDir["IMGUI"] = "Adonis/vendor/imgui"
IncludeDir["GLM"] = "Adonis/vendor/glm"

group "Dependencies"
	include "Adonis/vendor/GLFW"
	include "Adonis/vendor/glad"
	include "Adonis/vendor/imgui"
group ""

project "Adonis"
	location "Adonis"
	kind "SharedLib"
	language "C++"
	staticruntime "off"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "Adonis/src/pch.cpp"

	files{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.vert",
		"%{prj.name}/src/**.frag",
		"%{prj.name}/resources/WindowIcon.png"
	}

	includedirs{
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/stb/include",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/imgui",
		"%{prj.name}/vendor/jsonparser/single_include",
	}

	links{
		"GLFW",	
		"GLAD",
		"IMGUI",
		"opengl32.lib"
	}

	defines{
		"ADONIS_USE_GLFW",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"ADONIS_PLATFORM_WINDOWS",
			"ADONIS_BUILD_DLL",
			"ADONIS_GLLOADER_GLAD",
			"_CRT_SECURE_NO_WARNINGS",
			"STB_IMAGE_IMPLEMENTATION"
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		
	filter "configurations:Debug"
		defines "ADONIS_DEBUG"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		defines "ADONIS_RELEASE"
		runtime "Release"
		optimize "On"
		
	filter "configurations:Dist"
		defines "ADONIS_DIST"
		buildoptions "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp",
		"Adonis/resources/WindowIcon.png"
	}

	includedirs{
		"Adonis/src",
		"Adonis/vendor/spdlog/include",
		"Adonis/vendor/glm",
		"Adonis/vendor/stb/include",
		"Adonis/vendor/glad/include",
		"Adonis/vendor/GLFW/include",
		"Adonis/vendor/imgui/",
		"Adonis/vendor/jsonparser/single_include",
	}

	links{
		"Adonis",
		"IMGUI",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"ADONIS_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "ADONIS_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ADONIS_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ADONIS_DIST"
		buildoptions "Release"
		optimize "On"