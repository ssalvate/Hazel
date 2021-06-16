#pragma once

#include "Hazel/Core/Core.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	HZ_CORE_INFO("Entry Point");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif