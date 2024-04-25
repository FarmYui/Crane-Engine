workspace "CraneEngine"
    architecture "x64"
    startproject "Sandbox"

    configurations 
    {
        "Debug",
        "Relase",
        "Dist"
    }

    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "CraneEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "CraneEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "CraneEngine/vendor/ImGui"
IncludeDir["glm"] = "CraneEngine/vendor/glm"
IncludeDir["stb_image"] = "CraneEngine/vendor/stb_image"
IncludeDir["entt"] = "CraneEngine/vendor/entt/"
IncludeDir["yaml_cpp"] = "CraneEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "CraneEngine/vendor/ImGuizmo"

group "Dependencies"
    include "CraneEngine/vendor/GLFW"
    include "CraneEngine/vendor/Glad"
    include "CraneEngine/vendor/ImGui"
    include "CraneEngine/vendor/yaml-cpp"

group ""

project "CraneEngine"
    location "CraneEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "crpch.h"
    pchsource "CraneEngine/src/crpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/stb_image.cpp",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:CraneEngine/vendor/ImGuizmo/**.cpp"
        flags {"NoPCH"}

    filter "system:windows"
        systemversion "latest"
        
        defines
        {
            "CR_PLATFORM_WINDOWS",
            "CR_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
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


project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    
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
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}"
    }

    links 
    {
        "CraneEngine"
    }

    filter "system:windows"
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


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    
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
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }

    links 
    {
        "CraneEngine"
    }

    filter "system:windows"
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
