workspace "CraneEngine"
    architecture "x64"

    configurations 
    {
        "Debug",
        "Relase",
        "Dist"
    }

    startproject "Sandbox"


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "CraneEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "CraneEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "CraneEngine/vendor/ImGui"
IncludeDir["glm"] = "CraneEngine/vendor/glm"

group "Dependencies"
    include "CraneEngine/vendor/GLFW"
    include "CraneEngine/vendor/Glad"
    include "CraneEngine/vendor/ImGui"

group ""

project "CraneEngine"
    location "CraneEngine"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "crpch.h"
    pchsource "CraneEngine/src/crpch.cpp"

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
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
            "CR_PLATFORM_WINDOWS",
            "CR_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }
    
    filter "configurations:Debug"
        defines "CR_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Relase"
        defines "CR_RELASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CR_DIST"
        runtime "Release"
        optimize "On"


project "Sandbox"
    location "Sandbox"
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
        "CraneEngine/vendor/spdlog/include",
        "CraneEngine/src",
        "CraneEngine/vendor",
        "%{IncludeDir.glm}"
    }

    links 
    {
        "CraneEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
            "CR_PLATFORM_WINDOWS"
        }

    
    filter "configurations:Debug"
        defines "CR_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Relase"
        defines "CR_RELASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CR_DIST"
        runtime "Release"
        optimize "On"
