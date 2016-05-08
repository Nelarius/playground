
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
  _      __               __    __ 
 | | /| / /______ ___  __/ /___/ /_
 | |/ |/ / __/ -_) _ \/_  __/_  __/
 |__/|__/_/  \__/_//_/ /_/   /_/   
                                   
--]]
    project "wrenpp"
        kind "StaticLib"
        language "C++"
        targetdir "lib"
        files { "extern/wrenpp/src/**.cpp", "extern/wrenpp/src/**.h"  }
        includedirs { "extern/wrenpp/src", "extern/wren/src/include" }
        filter "configurations:Release"
            defines { "NDEBUG" }

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
        links { "wrenpp", "filesentry" }
        files { "src/**.cpp", "src/**.h", "extern/json11/json11.cpp", "extern/imgui/**.cpp", "data/**.wren", "data/**.glsl", "src/config.json" }
        includedirs {
            "src", "extern/wrenpp/src", "extern/filesentry/include",
            "extern", "extern/assimp/include", "extern/SDL/include",
            "extern/wren/src/include", "extern/glew-1.13.0/include"
        }
        --This should be enabled once all VC warnings get fixed:
        --flags { "FatalWarnings" } // This should be enabled once all VC warnings get fixed
        filter "configurations:Debug"
            debugdir "bin"
            links { "wren_static_d" }
            libdirs { "extern/wren/lib/Debug" }
        filter "configurations:Release or Test"
            links { "wren_static" }
            libdirs { "extern/wren/lib/Release" }
        project "engine"
            postbuildcommands {
                "{COPY} ../../src/config.json %{cfg.targetdir}",
                "{COPY} ../../extern/assimp/bin %{cfg.targetdir}",
                "{COPY} ../../extern/glew-1.13.0/bin %{cfg.targetdir}",
                "{COPY} ../../extern/SDL/bin %{cfg.targetdir}",
                "{COPY} ../../src/config.json %{cfg.targetdir}"
            }
            filter "files:**.wren"
                buildcommands { "{COPY} ../../data/wren/%{file.name} %{cfg.targetdir}/pg" }
                buildoutputs { "%{cfg.targetdir}/pg/%{file.name}" }
            filter "files:**glsl"
                buildcommands { "{COPY} ../../data/glsl/%{file.name} %{cfg.targetdir}/glsl" }
                buildoutputs { "%{cfg.targetdir}/glsl/%{file.name}" }
        --[[
  _   ___               __  ______          ___    
 | | / (_)__ __ _____ _/ / / __/ /___ _____/ (_)__ 
 | |/ / (_-</ // / _ `/ / _\ \/ __/ // / _  / / _ \
 |___/_/___/\_,_/\_,_/_/ /___/\__/\_,_/\_,_/_/\___/
                                                   
--]]
        configuration "vs*"
            defines { "_CRT_SECURE_NO_WARNINGS" } -- This is to turn off warnings about 'localtime'
            links { "SDL2", "assimp", "glew32", "opengl32"  }
            libdirs { 
                "extern/SDL/lib", "extern/assimp/lib",
                "extern/glew-1.13.0/lib"
            }
            prebuildcommands { 
                "if not exist \"..\\..\\bin\\pg\" mkdir ..\\..\\bin\\pg", 
                "if not exist \"..\\..\\bin\\glsl\" mkdir ..\\..\\bin\\glsl" 
            } 
