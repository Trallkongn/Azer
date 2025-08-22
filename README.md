# Azer GameEngine
---
# 目标
首先支持2D游戏的制作，尤其是针对对于Galgame制作的适配
然后是支持3D游戏的制作，最终的目标是针对大世界游戏的开发适配
一键光线追踪、DLSS、视频设置友好型、高性能的游戏引擎
同时支持CG、建模

# 项目架构
使用premake5（lua）来进行项目构建（未来会考虑使用cmake）
使用C++17标准进行开发
使用OpenGl作为图形接口，使用GLFW+Glad作为窗口接口
数学库：使用glm库
UI库：ImGui | Qt
引擎图形接口抽象->不同图形接口（OpenGl、Vulcan、Direct3d）
使用静态链接

请转至 3D 分支查看有关 3D 渲染的部分
