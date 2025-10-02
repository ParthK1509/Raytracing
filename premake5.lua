-- premake5.lua
workspace "Cashew"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Cashew"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "Cashew"