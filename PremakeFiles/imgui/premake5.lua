project "imgui"
  kind "StaticLib"
  language "C++"
  staticruntime "On"
  warnings "Off"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "imconfig.h",
    "imgui.h",
    "imgui.cpp",
    "imgui_draw.cpp",
    "imgui_tables.cpp",
    "imgui_widgets.cpp",
    "imgui_internal.h",
    "imgui_demo.cpp",
  }

  includedirs
  {
    "."
  }

  -- Windows 平台设置 
  filter { "system:windows" }
    systemversion "latest"  -- 使用最新 Windows SDK
 
  -- Debug 配置 
  filter "configurations:Debug"
    runtime "Debug"  -- 调试运行时库 
    symbols "On"     -- 生成调试符号
 
  -- Release 配置
  filter "configurations:Release"
    runtime "Release"  -- 发布运行时库
    optimize "On"      -- 启用优化
 
  -- Dist 配置（最终发布版）
  filter "configurations:Dist"
    runtime "Release"  -- 发布运行时库
    optimize "Full"    -- 完全优化（比 "On" 更激进）
    symbols "Off"      -- 不生成调试符号 