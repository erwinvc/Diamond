workspace "Diamond"
	architecture "x64"
	
	configurations { "Debug", "Release" }
	
	flags { "MultiProcessorCompile", "FatalCompileWarnings" }
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLAD"] = "engine/vendor/GLAD/include"
IncludeDir["GLFW"] = "engine/vendor/GLFW/include"

include "engine/vendor/GLAD"
include "engine/vendor/GLFW"

project "Diamond"
	location "engine"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	filter "system:windows"
		systemversion "latest"
		
	defines { "GLEW_STATIC"	}
	
	files
	{
		"engine/**.h",
		"engine/**.hpp",
		"engine/**.cpp",
		"engine/**.vert",
		"engine/**.frag",
		"engine/**.geom",
		"engine/**.tese",
		"engine/**.tesc",
		"engine/**.incl",
		"engine/**.rc"
	}
	
	filter { "files:engine/vendor/**.cpp" }
		flags "NoPCH"	
	filter {}
	
	pchheader "stdafx.h"
	pchsource "engine/src/stdafx.cpp"
	
	includedirs
	{
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLFW}",
		"engine/vendor/AssImp/include",
		"engine/vendor/GLFW/include",
		"engine/vendor/imgui",
		"engine/vendor",
		"engine/src",
		"engine"
	}
	
	libdirs  
	{ 
		"$(SolutionDir)engine/vendor/AssImp"
	}

	links
	{
		"GLFW",
		"GLAD",
		"opengl32"
	}
		
	configuration "vs*"
		defines     { "_CRT_SECURE_NO_WARNINGS" }

	filter "configurations:Debug"
		defines "D_DEBUG"
		runtime "Debug"
		symbols "On"

			libdirs  
			{ 
				"$(SolutionDir)engine/vendor/assimp/bin/Debug"
			}
			
			links
			{
				"assimp-vc141-mtd.lib",
				"zlibstatic"
			}
			
			postbuildcommands 
			{
				'{COPY} "../engine/vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
			}
			
	filter "configurations:Release"
		defines "D_RELEASE"
			flags{"LinkTimeOptimization"}
		runtime "Release"
		optimize "On"
		
			libdirs  
			{ 
				"$(SolutionDir)engine/vendor/assimp/bin/Release"
			}
			
			links
			{
				"assimp-vc141-mt.lib",
				"zlibstatic"
			}
			
			postbuildcommands 
			{
				'{COPY} "../engine/vendor/assimp/bin/Release/assimp-vc141-mt.dll" "%{cfg.targetdir}"'
			}