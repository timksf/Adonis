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

include "Adonis/vendor/GLFW"

project "Adonis"
	location "Adonis"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "Adonis/src/pch.cpp"

	files{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}",
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"ADONIS_PLATFORM_WINDOWS",
			"ADONIS_BUILD_DLL"
		}

		postbuildcommands{
			("{COPY} \"%{cfg.buildtarget.relpath}\" \"../bin/" .. outputdir .. "/Sandbox\"")
		}

		postbuildmessage ("copy %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		
	filter "configurations:Debug"
		defines "ADONIS_DEBUG"
		buildoptions "/MDd"
		symbols "On"
		
	filter "configurations:Release"
		defines "ADONIS_RELEASE"
		buildoptions "/MD"
		optimize "On"
		
	filter "configurations:Dist"
		defines "ADONIS_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"Adonis/src",
		"Adonis/vendor/spdlog/include"
	}

	links{
		"Adonis"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ADONIS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ADONIS_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ADONIS_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ADONIS_DIST"
		buildoptions "/MD"
		optimize "On"