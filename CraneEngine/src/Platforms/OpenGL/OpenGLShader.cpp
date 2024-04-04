#include "crpch.h"
#include "OpenGLShader.h"

#include <filesystem>

#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

namespace Crane
{

	static GLenum ShaderTypeFromString(const std::string& stringType)
	{
		if (stringType == "vertex")
			return GL_VERTEX_SHADER;
		if (stringType == "fragment" || stringType == "pixel")
			return GL_FRAGMENT_SHADER;

		CR_CORE_ASSERT(false, "Unknown string type!");
		return 0;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		CR_PROFILE_FUNCTION();
		std::string result;

		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				CR_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			CR_CORE_ERROR("filepath: {0} does not exist!", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		CR_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);

		size_t pos = source.find(typeToken, 0);//Start of shader type declaration line

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);//End of shader type declaration line
			CR_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;//Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			CR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			CR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		CR_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();
		CR_CORE_ASSERT(shaderSources.size() <= 2, "Too many shaders!");
		std::array<GLuint,2> shaderIDs;

		int glShaderIdIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& sSource = kv.second;
			
			GLuint shader = glCreateShader(type);
			const GLchar* source = (const GLchar*)sSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				CR_CORE_ERROR("{0}", infoLog.data());
				CR_CORE_ASSERT(false, "Shader compilation failed");
				return;
			}
			shaderIDs.at(glShaderIdIndex++) = shader;
			glAttachShader(program, shader);

		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (auto& id : shaderIDs)
				glDeleteShader(id);

			CR_CORE_ERROR("{0}", infoLog.data());
			CR_CORE_ASSERT(false, "Shader linking failed");
			return;
		}

		for (auto& id : shaderIDs)
		{
			glDetachShader(program,id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		CR_PROFILE_FUNCTION();
		std::string sources = ReadFile(filepath);
		auto shaderSources = PreProcess(sources);
		Compile(shaderSources);
	
		std::filesystem::path path = filepath;
		m_Name = path.stem().string();
	}

	OpenGLShader::OpenGLShader(const std::string& name ,const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		CR_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shaderSources);
	}

	
	OpenGLShader::~OpenGLShader()
	{
		CR_PROFILE_FUNCTION();
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		CR_PROFILE_FUNCTION();
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		CR_PROFILE_FUNCTION();
		glUseProgram(0);
	}


	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetInt2(const std::string& name, int* value)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformInt2(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, uint32_t count, int* value)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformIntArray(name, count, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		CR_PROFILE_FUNCTION();
		UploadUniformMat4(name, matrix);
	}


	//upload
	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformInt2(const std::string& name, int* value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, 2, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, uint32_t count, int* value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	
}
