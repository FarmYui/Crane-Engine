#pragma once

#include <glm/glm.hpp>

namespace Crane
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();
		
		void Bind() const;
		void Unìbind() const;
	
		void SetUniformMat4(const std::string& name, const glm::mat4& mat4) const;
	private:
		uint32_t m_RendererID;
	};
}