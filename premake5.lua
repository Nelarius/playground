
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

    group("external")
--[[
                    
 _    _________ ___ 
| |/|/ / __/ -_) _ \
|__,__/_/  \__/_//_/
                    
--]]
    project "wren"
        language "C"
        kind "StaticLib"
        files { "extern/wren/src/vm/**.c" }
        includedirs { "extern/wren/src/vm", "extern/wren/src/include" }
        -- the engine has it's own random number library
        -- meta module doesn't seem to have anything in it at the time of writing
        defines { "WREN_OPT_META=0", "WREN_OPT_RANDOM=0" }
        filter "action:vs*"
            defines { "_CRT_SECURE_NO_WARNINGS" } -- This is to turn off warnings about the C standard lib on Visual C compilers

--[[
                        __    __ 
 _    _________ ___  __/ /___/ /_
| |/|/ / __/ -_) _ \/_  __/_  __/
|__,__/_/  \__/_//_/ /_/   /_/   
                                 
--]]
    project "wren++"
        language "C++"
        kind "StaticLib"
        targetdir "lib"
        files { "extern/wrenpp/Wren++.cpp", "extern/wrenpp/Wren++.h" }
        includedirs { "extern/wrenpp/src", "extern/wren/src/include" }
        filter "configurations:Release"

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

    group "playground"
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
        links { "wren", "wren++", "filesentry" }
        files { "src/**.cpp", "src/**.h", "extern/imgui/**.cpp",
        "data/**.wren", "data/**.glsl", "src/config.json" }
        includedirs {
            "src", "extern/wrenpp", "extern/filesentry/include",
            "extern", "extern/assimp/include", "extern/SDL/include",
            "extern/wren/src/include", "extern/glew-1.13.0/include"
        }
        --This should be enabled once all VC warnings get fixed:
        --flags { "FatalWarnings" } // This should be enabled once all VC warnings get fixed
        links { "wren" }
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
        filter "configurations:Debug"
            debugdir "bin"
        --[[
  _   ___               __  ______          ___    
 | | / (_)__ __ _____ _/ / / __/ /___ _____/ (_)__ 
 | |/ / (_-</ // / _ `/ / _\ \/ __/ // / _  / / _ \
 |___/_/___/\_,_/\_,_/_/ /___/\__/\_,_/\_,_/_/\___/
                                                   
--]]
        filter "action:vs*"
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

    -- This requires that the engine is split out into a DLL and the main application is its own executable		
    -- Then the test application can just be linked with the engine		
    project "test"
        kind "ConsoleApp"
        language "C++"
        targetdir "bin"
        files { "test/**.cpp", "src/utils/**.cpp", "src/ecs/**.cpp" }
        includedirs { "src", "extern/unittest++", "extern" }
        configuration "vs*"
            defines { "_CRT_SECURE_NO_WARNINGS" } -- This is to turn off warnings about 'localtime'
        filter "configurations:Debug"
            debugdir "bin"
            links { "UnitTest++" }
            libdirs { "extern/unittest++/lib/Debug" }
        filter "configurations:Release"
            links { "UnitTest++" }
            libdirs { "extern/unittest++/lib/Release" }
