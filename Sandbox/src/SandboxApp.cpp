#include <Azer.h>

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