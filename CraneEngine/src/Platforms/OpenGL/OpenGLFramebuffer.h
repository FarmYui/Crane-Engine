#pragma once
#include "Crane/Renderer/Framebuffer.h"

namespace Crane
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& specification);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { CR_CORE_ASSERT(index < m_ColorAttachments.size(), "Invalid index, Index out of vector"); return m_ColorAttachments.at(index); }

		void Invalidate();
	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};
}