#pragma once
#include <Azer/Renderer/Framebuffer.h>

namespace Azer {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		void Resize(uint32_t width, uint32_t height) override;

		void Bind() const override;
		void Unbind() const override;

		uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};
}