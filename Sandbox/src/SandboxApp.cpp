#include <Azer.h>

class ExampleLayer : public Azer::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{

	}

	void OnUpdate() override
	{
		if (Azer::Input::IsKeyPressed(AZ_KEY_TAB))
		{
			AZ_INFO("Tab key is pressed");
		}
	}

	void OnEvent(Azer::Event& event) override
	{
		if (event.GetEventType() == Azer::EventType::KeyPressed)
		{
			Azer::KeyPressedEvent& e = (Azer::KeyPressedEvent&)event;
			AZ_INFO("{0}", (char)e.GetKeyCode());
		}
	}
};



class Sandbox : public Azer::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new Azer::ImGuiLayer()); 
	}

	~Sandbox()
	{

	}

};

Azer::Application* Azer::CreateApplication()
{
	return new Sandbox;
}