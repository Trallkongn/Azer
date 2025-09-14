#include <Azer.h>
#include <Azer/Core/EntryPoint.h>

#include "EditLayer.h"

namespace Azer {

	class AzerEditApp : public Azer::Application
	{
	public:
		AzerEditApp()
			: Application("Azer Editor")
		{
			//PushLayer(new ObjectCreationLayer());
			PushLayer(new EditLayer("EditLayer"));
		}

		~AzerEditApp()
		{

		}

	};

	Azer::Application* Azer::CreateApplication()
	{
		return new AzerEditApp;
	}
}

