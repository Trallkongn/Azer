#include <Azer.h>
#include <Azer/Core/EntryPoint.h>

class Sandbox : public Azer::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Azer::Application* Azer::CreateApplication()
{
	return new Sandbox;
}