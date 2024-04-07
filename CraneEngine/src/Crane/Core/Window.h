#pragma once
#include "crpch.h"

#include "Crane/Core/Core.h"
#include "Crane/Events/Event.h"
#include "Crane/Renderer/GraphicsContext.h"

namespace Crane
{
	struct WindowProps 
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Crane Engine App", 
					uint32_t width = 1280, 
					uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual float GetTime() const = 0;

		//window attribs
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool On) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}