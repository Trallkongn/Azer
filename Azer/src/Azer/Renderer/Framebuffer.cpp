#include "azpch.h"
#include "Framebuffer.h"
#include <Azer/Renderer/Renderer.h>

#include <Platform/OpenGL/OpenGLFramebuffer.h>

namespace Azer {

	Ref<Framebuffer> Azer::Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
		}

		AZ_CORE_ASSERT(false, "UnKnow RendererAPI!");
		return nullptr;
	}
}


