#pragma once
#include "Crane/Core/Core.h"


#ifdef CR_PLATFORM_WINDOWS

extern Crane::Application* Crane::CreateApplication();

int main(int argc, char** argv)
{
	Crane::Log::Init();

	CR_PROFILE_BEGIN_SESSION("Startup", "CraneProfile-Startup.json");
	auto app = Crane::CreateApplication();
	CR_PROFILE_END_SESSION();

	CR_PROFILE_BEGIN_SESSION("Runtime", "CraneProfile-Runtime.json");
	app->Run();
	CR_PROFILE_END_SESSION();

	CR_PROFILE_BEGIN_SESSION("Shutdown", "CraneProfile-Shutdown.json");
	delete app;
	CR_PROFILE_END_SESSION();
}

#endif


