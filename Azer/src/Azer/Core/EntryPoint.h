#pragma once

#ifdef AZ_PLATFORM_WINDOWS

extern Azer::Application* Azer::CreateApplication();

int main(int argc,char** argv)
{
	Azer::Log::Init();

	AZ_PROFILE_BEGIN_SESSION("Startup", "AzerProfile-Startup.json");
	auto app = Azer::CreateApplication();
	AZ_PROFILE_END_SESSION();

	AZ_PROFILE_BEGIN_SESSION("Runtime", "AzerProfile-Runtime.json");
	app->Run();
	AZ_PROFILE_END_SESSION();

	AZ_PROFILE_BEGIN_SESSION("Shutdown", "AzerProfile-Shutdown.json");
	delete app;
	AZ_PROFILE_END_SESSION();
}

#endif