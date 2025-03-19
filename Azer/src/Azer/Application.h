#pragma once

#include "azpch.h"

#include "Core.h"
#include "Azer/Events/Event.h"

namespace Azer {

	class AZER_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
