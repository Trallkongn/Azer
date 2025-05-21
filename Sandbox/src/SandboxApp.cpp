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
		AZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Azer::Event& event) override
	{
		AZ_TRACE("{0}", event.ToString());
	}
};

class Sandbox : public Azer::Application
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

Azer::Application* Azer::CreateApplication()
{
	return new Sandbox;
}