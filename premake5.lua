-- premake5.lua
workspace "srvr"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "srvr"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "srvr"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    vectorextensions "AVX2"
    systemversion "latest"

    targetdir ("bin/%{prj.name}-%{cfg.buildcfg}/out")
    objdir ("bin/%{prj.name}-%{cfg.buildcfg}/int")

    includedirs {
        "include",
    }

    files {
        "src/**.cpp"
    }

    links {
    }

    flags {
        "MultiProcessorCompile",
        "FatalWarnings"
    }

    filter "configurations:Debug"
        defines {
            "_DEBUG"
        }
        runtime "Debug"
        optimize "off"
        symbols "on"
        sanitize "Address"
    
    filter "configurations:Release"
        runtime "Release"
        optimize "speed"
        symbols "on"
        flags {
            "LinkTimeOptimization"
        }
    
    filter "configurations:Dist"
        runtime "Release"
        optimize "speed"
        symbols "off"
        flags {
            "LinkTimeOptimization"
        }

group "Dependencies"
group ""
