#pragma once


namespace Crane
{
	class Texture
	{
	public:
		virtual ~Texture() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}