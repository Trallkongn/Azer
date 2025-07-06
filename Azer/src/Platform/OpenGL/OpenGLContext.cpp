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

void Azer::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
