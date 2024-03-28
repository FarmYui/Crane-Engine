#pragma once

#include "Crane/Core/Core.h"
#include "Crane/Events/Event.h"
			   
#include "Crane/Core/Timestep.h"

namespace Crane
{
	class CRANE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}
		

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}