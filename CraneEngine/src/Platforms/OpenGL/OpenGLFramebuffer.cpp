#include "crpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Crane
{
    namespace Utils
    {
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multisampled, uint32_t* data, uint32_t count)
        {
            glCreateTextures(TextureTarget(multisampled), count, data);
        }

        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachColorTexture(uint32_t id, uint32_t samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, uint32_t index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
               
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, Utils::TextureTarget(multisampled), id, 0);

        }

        static void AttachDepthTexture(uint32_t id, uint32_t samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);

        }

        static bool isDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    return true;
            }
            return false;
        }


        static GLenum CraneTextureFormatToGL(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            }

            CR_CORE_ASSERT(false, "Unknown format");
            return 0;
        }

    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specification)
        : m_Specification(specification)
    {
        for (auto& format : m_Specification.Attachments.Attachments)
        {
            if (!Utils::isDepthFormat(format.TextureFormat))
                m_ColorAttachmentSpecifications.emplace_back(format);
            else
                m_DepthAttachmentSpecification = format;
        }
        Invalidate();

    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Specification.Width = width;
        m_Specification.Height = height;

        Invalidate();
    }

    int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        CR_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Invalid attachmentIndex, Reason: Index out of range");

        int pixelData;
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        return pixelData;
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        CR_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Invalid attachmentIndex, Reason: Index out of range");

        auto& spec = m_ColorAttachmentSpecifications.at(attachmentIndex);

        glClearTexImage(m_ColorAttachments.at(attachmentIndex), 0, Utils::CraneTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);

            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }



        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        
        bool multisample = m_Specification.Samples > 1;
        
        // Attachments
        if (m_ColorAttachmentSpecifications.size())
        {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            Utils::CreateTextures(multisample, m_ColorAttachments.data(), (uint32_t)m_ColorAttachments.size());

            for (uint32_t i = 0; i < m_ColorAttachments.size(); i++)
            {
                Utils::BindTexture(multisample, m_ColorAttachments.at(i));

                auto& specs = m_ColorAttachmentSpecifications.at(i);
                switch (specs.TextureFormat)
                {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_ColorAttachments.at(i), m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
                    break;

                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_ColorAttachments.at(i), m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
                    break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
        {
            Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
            Utils::BindTexture(multisample, m_DepthAttachment);

            switch (m_DepthAttachmentSpecification.TextureFormat)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
                break;
            }

        }
    
        if (m_ColorAttachments.size() > 1)
        {
            CR_CORE_ASSERT(m_ColorAttachments.size() <= 4, "Max Color Attachments is 4");
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers((int)m_ColorAttachments.size(), buffers);
        }
        else if (m_ColorAttachments.empty())
        {
            // Only depth pass
            glDrawBuffer(GL_NONE);
        }

        CR_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}