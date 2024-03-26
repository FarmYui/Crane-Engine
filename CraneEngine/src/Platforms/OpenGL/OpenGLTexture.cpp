#include "crpch.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

namespace Crane
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
        // load and create a texture 
        // -------------------------
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // load image, create texture and generate mipmaps
        stbi_set_flip_vertically_on_load(true);
        m_Buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 3);
       
        CR_CORE_ASSERT(m_Buffer, "Failed to load texture!");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(m_Buffer);
	}

	void OpenGLTexture2D::Bind() const
	{
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);/*something*/
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture2D::Unbind() const
	{
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);/*something*/
        glBindTexture(GL_TEXTURE_2D, 0);
	}
}