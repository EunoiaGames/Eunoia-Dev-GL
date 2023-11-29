workspace "Eunoia-Dev"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Eunoia-Engine"
	location "Eunoia-Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("Bin/"..outputdir.."/%{prj.name}")
	objdir ("Bin-Int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.hpp",
		"%{prj.name}/Src/**.inl",
		"%{prj.name}/Src/**.cpp",
		"%{prj.name}/Src/**.c"
	}

	includedirs
	{
		"%{prj.name}/Libs/spdlog/include",
		"%{prj.name}/Libs/GLEW/include",
		"%{prj.name}/Libs/GLFW/include"
	}

	links  
	{
		"Xinput9_1_0",
		"Eunoia-Engine/Libs/GLEW/glew32s",
		"Eunoia-Engine/Libs/GLFW/x64/glfw3",
		"opengl32"
	}

	defines
	{
		"EU_ENGINE",
		"GLEW_STATIC",
		"STB_IMAGE_IMPLEMENTATION"
	}

	--Generate Metadata
	prebuildcommands
	{
		"call \"../Bin/Dist-windows-x86_64/Eunoia-Introspection/Eunoia-Introspection.exe\" \"Src/Eunoia/Eunoia.h\" \"Src/Eunoia/Metadata/EunoiaGenerated.cpp\""
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0"

		defines
		{
			"EU_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			"{COPY} %{cfg.buildtarget.relpath} ../Bin/" .. outputdir .. "/Eunoia-Sandbox",
			"{COPY} %{cfg.buildtarget.relpath} ../Bin/" .. outputdir .. "/Eunoia-Editor",
		}

		links 
		{
			"Eunoia-Engine/Libs/GLEW/glew32s"
		}

	filter "configurations:Debug"
		defines "EU_DEBUG"
		symbols "On"
		buildoptions "/MDd"

		links 
		{
			"Eunoia-Engine/Libs/spdlog/spdlogd",
		}

	filter "configurations:Release"
		defines "EU_RELEASE"
		optimize "On"
		buildoptions "/MD"

		links 
		{
			"Eunoia-Engine/Libs/spdlog/spdlog",
		}
		
	filter "configurations:Dist"
		defines "EU_DIST"
		optimize "On"
		buildoptions "/MD"

		links 
		{
			"Eunoia-Engine/Libs/spdlog/spdlog",
		}

project "Eunoia-Sandbox"
	location "Eunoia-Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Bin/"..outputdir.."/%{prj.name}")
	objdir ("Bin-Int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.hpp",
		"%{prj.name}/Src/**.inl",
		"%{prj.name}/Src/**.cpp",
		"%{prj.name}/Src/**.c"	
	}

	includedirs
	{
		"Eunoia-Engine/Libs/spdlog/include",
		"Eunoia-Engine/Src",
		"Eunoia-Engine/Libs/GLFW/include"
	}

	links
	{
		"Eunoia-Engine",
		"Eunoia-Engine/Libs/GLFW/x64/glfw3_mt",
	}

	defines
	{
		"EU_PROJECT",
		"STB_IMAGE_IMPLEMENTATION"
	}

	postbuildcommands
	{		
		"{COPY} ../Bin/"..outputdir.."/Eunoia-Engine/Eunoia-Engine.dll %{cfg.buildtarget.directory}",
	}

	filter "configurations:Debug"
		defines "EU_DEBUG"
		buildoptions "/MTd" 

	filter "configurations:Release"
		defines "EU_RELEASE"
		buildoptions "/MT"

	filter "configurations:Dist"
		defines "EU_DIST"
		buildoptions "/MT"

project "Eunoia-TextureConverter"
	location "Eunoia-TextureConverter"
	kind  "ConsoleApp"
	language "C++"

	targetdir ("Bin/"..outputdir.."/%{prj.name}")
	objdir ("Bin-Int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",
	}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Dist"
		optimize "On"

project "Eunoia-Introspection"
	location "Eunoia-Introspection"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Bin/"..outputdir.."/%{prj.name}")
	objdir ("Bin-Int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",
	}

	filter "configurations:Debug"
		defines "EU_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "EU_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "EU_DIST"
		optimize "On"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0"

project "Eunoia-Editor"
	location "Eunoia-Editor"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Bin/"..outputdir.."/%{prj.name}")
	objdir ("Bin-Int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/Src/**.h",
		"%{prj.name}/Src/**.cpp",
	}

	includedirs
	{
		"Eunoia-Engine/Src",
		"Eunoia-Engine/Libs/spdlog/include",
		"Eunoia-Engine/Libs/GLFW/include",
		"Eunoia-Engine/Libs/GLEW/include",
	}

	links
	{
		"Eunoia-Engine",
		"Eunoia-Engine/Libs/GLFW/x64/glfw3"
	}

	defines
	{
		"EU_EDITOR",
		"STB_IMAGE_IMPLEMENTATION",
	}


	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0"

		defines
		{
			"EU_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{		
			"{COPY} ../Bin/"..outputdir.."/Eunoia-Engine/Eunoia-Engine.dll %{cfg.buildtarget.directory}",
		}

	filter "configurations:Debug"
		defines "EU_DEBUG"
		symbols "On"
		buildoptions "/MDd" 

	filter "configurations:Release"
		defines "EU_RELEASE"
		optimize "On"
		buildoptions "/MD" 

	filter "configurations:Dist"
		defines "EU_DIST"
		optimize "On"
		buildoptions "/MD" 