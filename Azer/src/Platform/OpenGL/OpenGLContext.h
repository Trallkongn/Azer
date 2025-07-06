#pragma once
#include "Azer/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Azer {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		// Extended by GraphicsContext 
		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}