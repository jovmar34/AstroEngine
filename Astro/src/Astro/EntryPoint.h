#pragma once

#ifdef AS_PLATFORM_WINDOWS

extern Astro::Application* Astro::CreateApplication();

int main(int argc, char** argv) {
	Astro::Log::Init();
	AS_CORE_WARN("Init Log!");

	int a = 1;
	AS_CORE_INFO("Hello, Var={0}", a);

	printf("Hello Engine!\n");
	auto app = Astro::CreateApplication();
	app->Run();
	delete app;
}

#endif