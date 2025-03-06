workspace "Azer"
  architecture "x64"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Azer"
  location "Azer"
  kind "SharedLib"
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
    "%{prj.name}/vendor/spdlog/include",
  }

  filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "10.0"

    defines
    {
      "AZ_PLATFORM_WINDOWS",
      "AZ_BUILD_DLL"
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
    defines "AZ_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "AZ_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "AZ_DIST"
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
    systemversion "10.0"

    defines
    {
      "AZ_PLATFORM_WINDOWS",
    }

  filter "configurations:Debug"
    defines "AZ_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "AZ_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "AZ_DIST"
    optimize "On"