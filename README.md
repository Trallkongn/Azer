# Azer
Azer Engine

注意：如果vs提示utf-8编码问题：

### **添加 `/utf-8` 编译选项**

这是最直接的解决方式。MSVC 编译器需要明确指定使用 UTF-8 编码来支持 Unicode 字符。操作步骤如下：

- **打开项目属性**：右键项目 → 选择 **属性** → 进入 **配置属性** → **C/C++** → **命令行**。
- **附加编译选项**：在 **其他选项** 框中添加 `/utf-8`，确保编译器以 UTF-8 编码处理源文件[2](https://blog.csdn.net/weixin_58796863/article/details/137962806)。
- **应用并重新编译**：保存设置并重新生成项目。

测试使用premake的 buildoptions 命令会导致生成dll文件卡死的问题... 目前还没有找到解决方法



# 2025 03 24 最新解决方法

Azer项目代码生成中启用 运行库 多线程调试 DLL
