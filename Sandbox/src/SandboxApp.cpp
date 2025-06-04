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
		//AZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(Azer::Event& e) override
	{
		//AZ_TRACE("{0}", e.ToString());
		//std::cout << "Hello!" << std::endl;
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