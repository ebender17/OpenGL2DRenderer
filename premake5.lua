workspace "OpenGL-Core"
    architecture "x64"
    startproject "OpenGL-Sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "OpenGL-Core/vendor/GLFW/include"
IncludeDir["Glad"] = "OpenGL-Core/vendor/Glad/include"
IncludeDir["ImGui"] = "OpenGL-Core/vendor/imgui"
IncludeDir["glm"] = "OpenGL-Core/vendor/glm"

include "OpenGL-Core/vendor/GLFW"
include "OpenGL-Core/vendor/Glad"
include "OpenGL-Core/vendor/imgui"

project "OpenGL-Core"
    location "OpenGL-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "glpch.h"
    pchsource "OpenGL-Core/src/glpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links 
    { 
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GLCORE_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "GLCORE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GLCORE_RELEASE"
        runtime "Release"
        optimize "on"

project "OpenGL-Sandbox"
    location "OpenGL-Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

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
        "OpenGL-Core/src",
        "OpenGL-Core/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "OpenGL-Core"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GLCORE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "GLCORE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GLCORE_RELEASE"
        runtime "Release"
        optimize "on"