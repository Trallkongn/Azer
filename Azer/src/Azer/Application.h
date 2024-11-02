#pragma once
#include "Core.h"

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
