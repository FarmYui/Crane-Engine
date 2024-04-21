#pragma once
#include "Crane/Core/Layer.h"

#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"

namespace Crane
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
	
		void AllowEvents(bool allow) { m_FlowEvents = allow; }
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetDarkThemeColors();
	private:
		bool m_FlowEvents = false;
		float m_Time = 0.0f;
	};
}