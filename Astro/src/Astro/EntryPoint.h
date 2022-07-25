#pragma once

#ifdef AS_PLATFORM_WINDOWS

extern Astro::Application* Astro::CreateApplication();

int main(int argc, char** argv) {
	printf("Hello Engine!\n");
	auto app = Astro::CreateApplication();
	app->Run();
	delete app;
}

#endif