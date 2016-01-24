
--[[
    Use these like so:
    premake5 vs2015 --wren=<location> --glew=<location> etc..
]]

workspace "playground"
    if _ACTION then
        location( "build/" .._ACTION )
    end

    configurations { "Debug", "Release", "Test" }

    -- global workspace configurations
    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "configurations:Test"
        defines { "DEBUG"}
        optimize "On"

    filter "action:gmake"
        buildoptions { "-std=gnu++14" }

    --[[
   ____ __                 __          
  / _(_) /__ ___ ___ ___  / /_______ __
 / _/ / / -_|_-</ -_) _ \/ __/ __/ // /
/_//_/_/\__/___/\__/_//_/\__/_/  \_, / 
                                /___/  
--]]
    project "filesentry"
        kind "StaticLib"
        language "C++"
        targetdir "lib"
        files { "extern/filesentry/source/FileWatcher.cpp" }
        includedirs { "extern/filesentry/include" }
        filter "system:windows"
            files { "extern/filesentry/source/FileWatcherWin32.cpp" }
        filter "system:macosx"
            files { "extern/filesentry/source/FileWatcherOSX.cpp" }
        filter "system:linux"
            files { "extern/filesentry/source/FileWatcherLinux.cpp" }

    --[[
                      __    
 _    _________ ___  / /_ __
| |/|/ / __/ -_) _ \/ / // /
|__,__/_/  \__/_//_/_/\_, / 
                     /___/  
--]]
    project "wrenly"
        kind "StaticLib"
        language "C++"
        targetdir "lib"

        files { "extern/wrenly/src/**.cpp", "extern/wrenly/src/**.h"  }
        includedirs { "extern/wrenly/src", "extern/wren/src/include" }


    --[[
                _         
 ___ ___  ___ _(_)__  ___ 
/ -_) _ \/ _ `/ / _ \/ -_)
\__/_//_/\_, /_/_//_/\__/ 
        /___/             
--]]
    project "engine"
        kind "ConsoleApp"
        language "C++"
        targetdir "bin"
        links { "wrenly", "filesentry" }
        files { "src/**.cpp", "src/**.h", "extern/json11/json11.cpp", "extern/imgui/**.cpp", "builtin/**.wren", "builtin/**.glsl" }
        includedirs {
            "src", "extern/wrenly/src", "extern/filesentry/include",
            "extern", "extern/assimp/include", "extern/SDL/include",
            "extern/wren/src/include", "extern/glew-1.13.0/include"
        }
        filter "configurations:Debug"
            debugdir "bin"
            links { "wren_static_d" }
            libdirs { "extern/wren/lib/Debug" }
        filter "configurations:Release or Test"
            links { "wren_static" }
            libdirs { "extern/wren/lib/Release" }
        project "engine"
        --This should be enabled once all VC warnings get fixed:
        --flags { "FatalWarnings" } // This should be enabled once all VC warnings get fixed
        --[[
  _   ___               __  ______          ___    
 | | / (_)__ __ _____ _/ / / __/ /___ _____/ (_)__ 
 | |/ / (_-</ // / _ `/ / _\ \/ __/ // / _  / / _ \
 |___/_/___/\_,_/\_,_/_/ /___/\__/\_,_/\_,_/_/\___/
                                                   
--]]
        configuration "vs*"
            defines { "_CRT_SECURE_NO_WARNINGS" } -- This is to turn off warnings about 'localtime'
            prebuildcommands {
                "if not exist \"..\\..\\bin\\builtin\" mkdir ..\\..\\bin\\builtin"
            }
            links { "SDL2", "assimp", "glew32", "opengl32"  }
            libdirs { 
                "extern/SDL/VisualC/Win32/Release", "extern/assimp/build/code/Release", 
                "extern/glew-1.13.0/lib/Release/Win32"
            }
            postbuildcommands {
                "copy ..\\..\\src\\config.json ..\\..\\bin",
            }
            -- exlcude *.wren files from build, just copy them to the correct directory
            filter "files:**.wren or **.glsl"
                --buildmessage "Copying %{file.relpath}..."
                buildcommands { "copy ..\\..\\builtin\\%{file.name} ..\\..\\bin\\builtin" }
                buildoutputs { "..\\..\\bin\\builtin\\%{file.name}" }
            project "engine"
        --[[
   __  ___     __       ____ __   
  /  |/  /__ _/ /_____ / _(_) /__ 
 / /|_/ / _ `/  '_/ -_) _/ / / -_)
/_/  /_/\_,_/_/\_\\__/_//_/_/\__/ 
                                  
--]]
        configuration "gmake"
            -- Mac and Linux support go here
            prebuildcommands {
                "mkdir -p ../../bin/data",
                "mkdir -p ../../bin/builtin"
            }
            postbuildcommands {
                "cp ../../config.json ../../bin",
                "cp -rf ../../src/data ../../bin"
            }
            filter "files:**.wren or **.glsl"
                buildcommands { "cp ../../builtin/%{file.name} ../../bin/builtin" }
                buildoutputs { "../../bin/builtin/%{file.name}" }
            project "engine"

    -- This requires that the engine is split out into a DLL and the main application is its own executable
    -- Then the test application can just be linked with the engine
    --[[project "tests"
        kind "ConsoleApp"
        language "C++"
        targetdir "bin"
        files { "test/**.cpp" }
        includedirs { "src" }

        configuration "vs*"
            links { "UnitTest++" }
            libdirs { "extern/unittest++/build/Release" }]]
