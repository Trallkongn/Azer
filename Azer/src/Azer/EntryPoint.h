#pragma once

#ifdef AZ_PLATFORM_WINDOWS

extern Azer::Application* Azer::CreateApplication();

int main(int argc,char** argv)
{
	Azer::Log::Init();
	AZ_CORE_WARN("Initialized Log!");
	int a = 5;
	AZ_INFO("Welcome!",a);

	auto app = Azer::CreateApplication();
	app->Run();
	delete app;
}

#endif