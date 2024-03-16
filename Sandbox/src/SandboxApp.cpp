#include <Crane.h>

class ExampleLayer : public Crane::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override
	{
		if (Crane::Input::IsKeyPressed(CR_KEY_SPACE))
			CR_TRACE("Space key is Pressed");
	}

	void OnEvent(Crane::Event& e) override
	{
		if (e.GetEventType() == Crane::EventType::KeyPressed)
		{
			Crane::KeyPressedEvent& ev = (Crane::KeyPressedEvent&)e;
			CR_TRACE("{0}", (char)ev.GetKeyCode());
		}
	}

};

class Sandbox : public Crane::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}

};


Crane::Application* Crane::CreateApplication() 
{
	return new Sandbox();
}