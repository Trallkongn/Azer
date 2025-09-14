#pragma once

#include <Azer/Core/Core.h>

namespace Azer {

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
		// glBindFrameBuffer(0);
	};

	class Framebuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		//virtual FrameBufferSpecification& GetSpecification() = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}