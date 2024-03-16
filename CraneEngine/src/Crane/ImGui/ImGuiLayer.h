#pragma once
#include "Crane/Layer.h"

#include "Crane/Application.h"

#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"

namespace Crane
{
	class CRANE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}