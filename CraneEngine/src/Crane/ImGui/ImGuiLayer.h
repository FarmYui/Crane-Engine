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

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}