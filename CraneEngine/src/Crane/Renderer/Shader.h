#pragma once

#include <glm/glm.hpp>

namespace Crane
{
	class Shader
	{
	public:
		virtual ~Shader() = default;
		
		virtual void Bind() const = 0;
		virtual void Unìbind() const = 0;
	
		static Ref<Shader> Create(const std::string& vertexSource, const std::string& fragmentSource);
	
	};
}