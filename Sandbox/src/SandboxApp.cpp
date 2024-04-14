#include "Sandbox2D.h"
#include "Crane/Core/EntryPoint.h"

class Sandbox : public Crane::Application
{
public:
	Sandbox()
	{
		PushLayer(new Crane::Sandbox2D());
	}
	~Sandbox()
	{

	}

};


Crane::Application* Crane::CreateApplication() 
{
	return new Sandbox();
}