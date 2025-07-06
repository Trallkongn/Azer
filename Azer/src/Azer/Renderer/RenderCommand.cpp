#include "azpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Azer {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}