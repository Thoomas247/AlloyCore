-- Config --
OUTPUT_DIR = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" -- e.g. Debug-Windows-x86_64

-- Plugin Workspace --
workspace ("AlloyCore_workspace")
	architecture "x64"

	startproject "Test"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

-- Include Directories Table --
IncludeDirs = {}

IncludeDirs["Plugin"] = "lib/AlloyCore/include"
IncludeDirs["External"] = "lib/AlloyCore/external"
IncludeDirs["Test"] = "lib/Test/src"

-- Global Macro Definitions --
defines 
{ 
	"_CRT_SECURE_NO_WARNINGS",   		   			-- some external projects use unsafe stl functions, remove the warning
	"NOGDI",				   						-- WinGDI.h defines dumb macros, remove it
	"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING" -- spdlog causes warnings, suppress them
}

-- Config Specific Settings --
filter "configurations:Debug"
	defines "AL_DEBUG"
	runtime "Debug"
	symbols "On"

filter "configurations:Release"
	defines "AL_RELEASE"
	runtime "Release"
	optimize "On"

filter "configurations:Dist"
	defines "AL_DIST"
	runtime "Release"
	optimize "On"

-- Plugin Project --
project "AlloyCore"
	location ("lib/AlloyCore")

	kind "StaticLib"
	staticruntime "On"

	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir ("int/" .. OUTPUT_DIR .. "/%{prj.name}")

	files
	{
		"lib/AlloyCore/src/**.h",
		"lib/AlloyCore/src/**.c",
		"lib/AlloyCore/src/**.hpp",
		"lib/AlloyCore/src/**.cpp"
	}

	includedirs
	{
		IncludeDirs["Plugin"],
		IncludeDirs["External"]
	}

    -- system specific macro definitions
	filter "system:Windows"
		defines "AL_SYSTEM_WINDOWS"

	filter "system:Unix"
      	defines "AL_SYSTEM_LINUX"

   	filter "system:Mac"
      	defines "AL_SYSTEM_MAC"

-- Test Project --
project "Test"
	location "lib/Test"
	
	kind "ConsoleApp"
	staticruntime "On"

	language "C++"
	cppdialect "C++20"

	targetdir ("bin/" .. OUTPUT_DIR .. "/%{prj.name}")
	objdir ("int/" .. OUTPUT_DIR .. "/%{prj.name}")

	links
	{
		"AlloyCore"
	}
	
	files
	{
		"lib/Test/src/**.h",
		"lib/Test/src/**.c",
		"lib/Test/src/**.hpp",
		"lib/Test/src/**.cpp"
	}

	includedirs
	{
		IncludeDirs["Plugin"],
		IncludeDirs["External"],
		IncludeDirs["Test"]
	}
