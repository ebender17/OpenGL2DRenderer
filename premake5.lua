workspace "OpenGL-Core"
	architecture "x64"
	startproject "OpenGL-Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "OpenGL-Core/vendor/GLFW/include"

include "OpenGL-Core/vendor/GLFW"

project "OpenGL-Core"
	location "OpenGL-Core"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "glpch.h"
	pchsource "OpenGL-Core/src/glpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		runtime "Debug"
		systemversion "latest"

		defines
		{
			"GLCORE_PLATFORM_WINDOWS",
			"GLCORE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/OpenGL-Sandbox")
		}

	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GLCORE_DIST"
		optimize "On"

project "OpenGL-Sandbox"
	location "OpenGL-Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"OpenGL-Core/vendor/spdlog/include",
		"OpenGL-Core/src"
	}

	links
	{
		"OpenGL-Core"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GLCORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "GLCORE_DIST"
		optimize "On"