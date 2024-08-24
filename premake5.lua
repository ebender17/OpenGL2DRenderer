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
IncludeDir["Glad"] = "OpenGL-Core/vendor/Glad/include"

include "OpenGL-Core/vendor/GLFW"
include "OpenGL-Core/vendor/Glad"

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
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}"
    }

    links
    {
        "GLFW",
        "Glad",
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
            "GLCORE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/OpenGL-Sandbox")
        }

    filter "configurations:Debug"
        defines "GLCORE_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "GLCORE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "GLCORE_DIST"
        buildoptions "/MD"
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
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "GLCORE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "GLCORE_DIST"
        buildoptions "/MD"
        optimize "On"