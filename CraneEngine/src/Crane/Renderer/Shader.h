#pragma once

namespace Crane
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unìbind() const;
	private:
		uint32_t m_RendererID;
	};
}