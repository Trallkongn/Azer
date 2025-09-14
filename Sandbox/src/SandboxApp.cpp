#include <Azer.h>
#include <Azer/Core/EntryPoint.h>

#include "NewLayer.h"

class Sandbox : public Azer::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ObjectCreationLayer());
		PushLayer(new NewLayer("NewLayer"));
	}

	~Sandbox()
	{

	}

};

Azer::Application* Azer::CreateApplication()
{
	return new Sandbox;
}