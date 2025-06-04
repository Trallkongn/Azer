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

include "Azer/vendor/GLFW"
include "Azer/vendor/Glad"
include "Azer/vendor/imgui"

project "Azer"
  location "Azer"
  kind "SharedLib"
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "azpch.h"
  pchsource "Azer/src/azpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}"
  }

  links
  {
    "GLFW",
    "GLAD",
    "ImGui",
    "opengl32.lib"
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "AZ_PLATFORM_WINDOWS",
      "AZ_BUILD_DLL",
      "AZ_ENABLE_ASSERTS",
      "GLFW_INCLUDE_NONE"
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
    defines "AZ_DEBUG"
    buildoptions {"/MDd","/utf-8"}
    symbols "On"

  filter "configurations:Release"
    defines "AZ_RELEASE"
    buildoptions {"/MD","/utf-8"}
    optimize "On"

  filter "configurations:Dist"
    defines "AZ_DIST"
    buildoptions {"/MD","/utf-8"}
    optimize "On"
  
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"


    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "Azer/vendor/spdlog/include",
    "Azer/src"
  }

  links
  {
    "Azer"
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "AZ_PLATFORM_WINDOWS",
    }

  filter "configurations:Debug"
    defines "AZ_DEBUG"
    buildoptions {"/MTd","/utf-8"}
    symbols "On"

  filter "configurations:Release"
    defines "AZ_RELEASE"
    buildoptions {"/MT","/utf-8"}
    optimize "On"

  filter "configurations:Dist"
    defines "AZ_DIST"
    buildoptions {"/MT","/utf-8"}
    optimize "On"