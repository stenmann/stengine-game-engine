workspace "game_engine"
   configurations { "Debug", "Release" }
   platforms { "x64" }

project "game_engine"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "x64/%{cfg.buildcfg}"

   files { 
      -- engine
      "src/*.h", 
      "src/*.hpp",
      "src/*.cpp",

      -- glm
      "glm/glm/**.hpp",
      "glm/glm/**.inl",

      -- box2d (IMPORTANT)
      "box2d/collision/**.cpp",
      "box2d/common/**.cpp",
      "box2d/dynamics/**.cpp",
      "box2d/rope/**.cpp"
   }

   removefiles {
      "glm/test/**",
      "glm/glm/detail/glm.cpp"
   }

   includedirs {
      ".",
      "src",
      "glm",
      "rapidjson/include",
      "lua",
      "LuaBridge",

      -- critical for your structure
      "box2d"
   }

   ---------------------------------------
   -- WINDOWS ONLY
   ---------------------------------------
   filter "system:windows"
      defines { "SDL_MAIN_HANDLED" }

      files { "lua/*.c" }
      removefiles {
         "lua/lua.c",
         "lua/luac.c"
      }

      includedirs {
         "SDL2",
         "SDL2_image",
         "SDL2_ttf",
         "SDL2_mixer"
      }

      libdirs { 
         "SDL2/lib/x64",
         "SDL2_image/lib/x64",
         "SDL2_ttf/lib/x64",
         "SDL2_mixer/lib/x64"
      }

      links { 
         "SDL2",
         "SDL2main",
         "SDL2_image",
         "SDL2_ttf",
         "SDL2_mixer"
      }

      postbuildcommands {
         "{COPY} %{wks.location}/SDL2/lib/x64/*.dll %{cfg.targetdir}",
         "{COPY} %{wks.location}/SDL2_image/lib/x64/*.dll %{cfg.targetdir}",
         "{COPY} %{wks.location}/SDL2_ttf/lib/x64/*.dll %{cfg.targetdir}",
         "{COPY} %{wks.location}/SDL2_mixer/lib/x64/*.dll %{cfg.targetdir}"
      }

   ---------------------------------------
   -- FLAGS
   ---------------------------------------
   filter "action:vs*"
      buildoptions { "/wd4996" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"