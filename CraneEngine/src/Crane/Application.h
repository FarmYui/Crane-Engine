#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Crane 
{
	class CRANE_API Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	//To define in client
	Application* CreateApplication();
}