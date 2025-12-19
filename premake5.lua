workspace "IstuMst3"
    configurations { "Debug", "Release" }
    filter "not action:gmake"
        location(path.join(_WORKING_DIR, "project"))


project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++Latest"
    targetdir "bin/%{cfg.buildcfg}"

    newoption {
        trigger = "build_switch",
        value = "value",
        description = "",
        allowed = {
            { "tests", "Tests" },
            { "benchmarks", "Benchmarks" },
        },
        default = "benchmarks"
    }

    includedirs { "./" }

    files { "mst/**.cpp", "mst/**.hpp" }

    buildoptions { 
        "-Wall", 
        "-O2",
    }

    filter "options:build_switch=tests"
        files { "tests/**.cpp", "tests/**.hpp" }
        filter "system:linux or macos"
            defines { "COLORTERMINAL" }

    filter "options:build_switch=benchmarks"
        files { "benchmarks/**.cpp", "benchmarks/**.hpp" }


    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        
    -- TEMP.
    -- filter "system:windows"
    --     defines { "WIN32" }
    
