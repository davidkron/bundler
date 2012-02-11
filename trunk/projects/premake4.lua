solution "Bundler"

	language "C++"
	location ( os.get() .. "/" .. _ACTION )
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE" }
	targetdir ( "../bin/" .. os.get() )
	includedirs { "../include/" }
	
	configurations
	{ 
		"Release",
		"Debug"
	}

configuration "Release"
	defines { "NDEBUG" }
	flags{ "Optimize", "FloatFast" }
	
configuration "Debug"
	defines { "_DEBUG" }
	targetsuffix "_d"

project "Bundler"
	files { "../src/bundler/**.*" }
	kind "WindowedApp"
	

	configuration "Release"
		targetname( "bundler" )
		links( "bootil_s" )
		
		
	configuration "Debug"
		targetname( "bundler_d" )
		links( "bootil_sd" )

project "Launcher"
	files { "../src/launcher/**.*" }
	kind "WindowedApp"
	links( "bootil_s" )
	
	configuration "Release"
		targetname( "launcher" )
		links( "bootil_s" )
		
	configuration "Debug"
		targetname( "launcher_d" )
		links( "bootil_sd" )