workspace "Astro"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include dirs relative to root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Astro/vendor/GLFW/include"
IncludeDir["Glad"] = "Astro/vendor/Glad/include"
IncludeDir["ImGui"] = "Astro/vendor/imgui"
IncludeDir["glm"] = "Astro/vendor/glm"

include "Astro/vendor/GLFW"
include "Astro/vendor/Glad"
include "Astro/vendor/ImGui"

project "Astro"
	location "Astro"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "aspch.h"
	pchsource "Astro/src/aspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/glm/glm/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AS_PLATFORM_WINDOWS",
			"AS_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

	filter "configurations:Debug"
		defines "AS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AS_DIST"
		runtime "Release"
		optimize "on"

		
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Astro/vendor/spdlog/include",
		"Astro/src",
		"Astro/vendor/imgui",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Astro"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "AS_DEBUG"
		runtime "Debug"		
		symbols "on"

	filter "configurations:Release"
		defines "AS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AS_DIST"
		runtime "Release"
		symbols "off"
		optimize "on"