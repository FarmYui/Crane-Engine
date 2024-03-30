#pragma once
#include "Crane/Core/Core.h"


#ifdef CR_PLATFORM_WINDOWS

extern Crane::Application* Crane::CreateApplication();

int main(int argc, char** argv)
{
	Crane::Log::Init();
	CR_CORE_INFO("HI!");
	CR_WARN("Initialized Logger");

	auto app = Crane::CreateApplication();
	app->Run();
	delete app;
}

#endif


