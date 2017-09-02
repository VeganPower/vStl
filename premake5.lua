-- premake5.lua
workspace "vStl"
   configurations { "Debug", "Release" }

project "vStlTest"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   includedirs './src/'
   cppdialect 'C++14'
   files { "test/**.h", "test/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"