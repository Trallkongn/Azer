#include "azpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

Azer::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	AZ_CORE_ASSERT(windowHandle, "Handle is null!")
}

void Azer::OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);

	// Init Glad
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	AZ_CORE_ASSERT(status, "Failed to initialize Glad!");
	char* vendor = (char*)glGetString(GL_VENDOR);
	char* renderer = (char*)glGetString(GL_RENDERER);
	char* version = (char*)glGetString(GL_VERSION);
	AZ_CORE_INFO("OpenGL Info:");
	AZ_CORE_INFO("	Vendor:	  {0}", vendor);
	AZ_CORE_INFO("	Renderer: {0}", renderer);
	AZ_CORE_INFO("	Version:  {0}", version);
}

#ifdef AZ_ENABLE_ASSERT
	int	majorVersion;
	int minorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	AZ_CORE_ASSERT(majorVersion > 4 || (majorVersion==4 && minorVersion >=5), "Azer requires at least OpenGL version 4.5!");
#endif

void Azer::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
