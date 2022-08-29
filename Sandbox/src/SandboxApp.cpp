#include <Astro.h>

#include <imgui.h>

class ExampleLayer : public Astro::Layer
{
public:
	ExampleLayer()
		: Layer("Example") 
	{
	}

	void OnUpdate() override
	{
		if (Astro::Input::IsKeyPressed(AS_KEY_TAB))
			AS_INFO("Tab is pressed! (poll)");
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Astro::Event& event) override
	{
		if (event.GetEventType() == Astro::EventType::KeyPressed)
		{
			Astro::KeyPressedEvent& e = (Astro::KeyPressedEvent&)event;
			AS_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Astro::Application 
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Astro::Application* Astro::CreateApplication() {
	return new Sandbox();
}