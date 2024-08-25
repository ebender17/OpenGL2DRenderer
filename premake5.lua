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
IncludeDir["ImGui"] = "OpenGL-Core/vendor/imgui"
IncludeDir["glm"] = "OpenGL-Core/vendor/glm"

include "OpenGL-Core/vendor/GLFW"
include "OpenGL-Core/vendor/Glad"
include "OpenGL-Core/vendor/imgui"

project "OpenGL-Core"
    location "OpenGL-Core"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

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
        cppdialect "C++20"
        systemversion "latest"

        defines
        {
            "GLCORE_PLATFORM_WINDOWS",
            "GLCORE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/OpenGL-Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "GLCORE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "GLCORE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "GLCORE_DIST"
        runtime "Release"
        optimize "On"

project "OpenGL-Sandbox"
    location "OpenGL-Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

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
        cppdialect "C++20"
        systemversion "latest"

        defines
        {
            "GLCORE_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "GLCORE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "GLCORE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "GLCORE_DIST"
        runtime "Release"
        optimize "On"