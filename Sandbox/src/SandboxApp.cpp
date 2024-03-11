#include <Crane.h>

class ExampleLayer : public Crane::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override
	{
		//CR_INFO("ExampleLayer::Update");
	}

	void OnEvent(Crane::Event& e) override
	{
		//CR_TRACE("{0}", e);
	}

};


class Sandbox : public Crane::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Crane::ImGuiLayer());
	}
	~Sandbox()
	{

	}

};


Crane::Application* Crane::CreateApplication() 
{
	return new Sandbox();
}