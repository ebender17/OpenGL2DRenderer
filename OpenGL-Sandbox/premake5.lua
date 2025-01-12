project "OpenGL-Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "../OpenGL-Core/src",
        "../OpenGL-Core/%{IncludeDir.GLFW}",
        "../OpenGL-Core/%{IncludeDir.Glad}",
        "../OpenGL-Core/%{IncludeDir.glm}",
        "../OpenGL-Core/%{IncludeDir.ImGui}",
        "../OpenGL-Core/vendor",
        "../OpenGL-Core/vendor/spdlog/include",
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