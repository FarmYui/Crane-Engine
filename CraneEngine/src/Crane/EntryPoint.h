#pragma once
#include "Application.h"

#ifdef CR_PLATFORM_WINDOWS

extern Crane::Application* Crane::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Crane::CreateApplication();
	app->Run();
	delete app;
}

#endif


