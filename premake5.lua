workspace "Azer"
  architecture "x64"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Azer/vendor/GLFW/include"
IncludeDir["Glad"] = "Azer/vendor/Glad/include"
IncludeDir["ImGui"] = "Azer/vendor/imgui"
IncludeDir["glm"] = "Azer/vendor/glm"
IncludeDir["stb_image"] = "Azer/vendor/stb_image"

include "Azer/vendor/GLFW"
include "Azer/vendor/Glad"
include "Azer/vendor/imgui"

project "Azer"
  location "Azer"
  kind "StaticLib"
  language "C++"
  staticruntime "On"
  cppdialect "C++17"
  buildoptions { "/utf-8"}

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "azpch.h"
  pchsource "Azer/src/azpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/vendor/glm/glm/**.hpp",
    "%{prj.name}/vendor/glm/glm/**.h",
    "%{prj.name}/vendor/glm/glm/**.inl",
    "%{prj.name}/vendor/stb_image/**.h",
    "%{prj.name}/vendor/stb_image/**.cpp",
  }

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.stb_image}"
  }

  links
  {
    "GLFW",
    "GLAD",
    "ImGui",
    "opengl32.lib"
  }

  filter "system:windows"
    systemversion "latest"

    defines
    {
      "AZ_PLATFORM_WINDOWS",
      "AZ_BUILD_DLL",
      "AZ_ENABLE_ASSERTS",
      "GLFW_INCLUDE_NONE"
    }

  filter "configurations:Debug"
    defines "AZ_DEBUG"
    runtime "Debug"
    symbols "On"

  filter "configurations:Release"
    defines "AZ_RELEASE"
    runtime "Release"
    optimize "On"

  filter "configurations:Dist"
    defines "AZ_DIST"
    runtime "Release"
    optimize "Full"
    symbols "Off"
  
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
    buildoptions { "/utf-8"}

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
  }

  includedirs
  {
    "Azer/vendor/spdlog/include",
    "Azer/src",
    "Azer/vendor",
    "%{IncludeDir.glm}",
  }

  links
  {
    "Azer"
  }

  filter "system:windows"
    systemversion "latest"

    defines
    {
      "AZ_PLATFORM_WINDOWS"
    }

  filter "configurations:Debug"
    defines "AZ_DEBUG"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    defines "AZ_RELEASE"
    runtime "Release"
    optimize "on"

  filter "configurations:Dist"
    defines "AZ_DIST"
    runtime "Release"
    optimize "Full"
    symbols "off"