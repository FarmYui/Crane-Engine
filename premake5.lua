workspace "CraneEngine"
    architecture "x64"

    configurations 
    {
        "Debug",
        "Relase",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CraneEngine"
    location "CraneEngine"
    kind "SharedLib"
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
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
            "CR_PLATFORM_WINDOWS",
            "CR_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "CR_DEBUG"
        symbols "On"

    filter "configurations:Relase"
        defines "CR_RELASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CR_DIST"
        optimize "On"


project "Sandbox"
    location "Sandbox"
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
        "CraneEngine/vendor/spdlog/include",
        "CraneEngine/src"
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
        symbols "On"

    filter "configurations:Relase"
        defines "CR_RELASE"
        optimize "On"
    
    filter "configurations:Dist"
        defines "CR_DIST"
        optimize "On"
