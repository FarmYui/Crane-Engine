#include "EditorLayer.h"
#include "Crane/Core/EntryPoint.h"

namespace Crane
{
	class EditorApp : public Application
	{
	public:
		EditorApp()
		{
			PushLayer(new EditorLayer());
		}
		~EditorApp()
		{

		}

	};


	Application* CreateApplication() 
	{
		return new EditorApp();
	}
}
