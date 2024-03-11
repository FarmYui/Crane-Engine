#pragma once
#include "Crane/Layer.h"

#include "Crane/Application.h"

namespace Crane
{
	class CRANE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);

	private:
		float m_Time = 0.0f;
	};
}