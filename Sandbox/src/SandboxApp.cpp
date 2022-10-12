#include <Astro.h>

#include <imgui.h>
#include <glm/gtx/transform.hpp>
#include <Platform/OpenGL/OpenGLShader.h>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Astro::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_CameraPosition = m_Camera.GetPosition();
		m_CameraRotation = m_Camera.GetRotation();
		m_ObjRotation = 0.0f;

		m_VertexArray.reset(Astro::VertexArray::Create());

		float vertices[(3 + 4) * 3] = {
			-0.5f,	-0.5f,	0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f,	-0.5f,	0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f,	0.5f,	0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		Astro::Ref<Astro::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Astro::VertexBuffer::Create(vertices, sizeof(vertices)));
		{
			Astro::BufferLayout layout = {
				{ Astro::ShaderDataType::Float3, "a_Position"},
				{ Astro::ShaderDataType::Float4, "a_Color"}
			};

			vertexBuffer->SetLayout(layout);
		}
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Astro::Ref<Astro::IndexBuffer> indexBuffer;
		indexBuffer.reset(Astro::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.5f,	-0.5f,	0.0f,
			 0.5f,	-0.5f,	0.0f,
			 0.5f,	 0.5f,	0.0f,
			-0.5f,	 0.5f,	0.0f,
		};

		m_SquareVA.reset(Astro::VertexArray::Create());
		Astro::Ref<Astro::VertexBuffer> squareVB;
		squareVB.reset(Astro::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Astro::ShaderDataType::Float3, "a_Position"}
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 0, 2, 3 };
		Astro::Ref<Astro::IndexBuffer> squareIB;
		squareIB.reset(Astro::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			in vec4 v_Color;

			uniform mat4 u_ViewProjection;

			layout(location = 0) out vec4 color;

			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(Astro::Shader::Create(vertexSrc, fragmentSrc));

		std::string positionShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform vec3 u_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string positionShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;
			uniform mat4 u_ViewProjection;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Astro::Shader::Create(positionShaderVertexSrc, positionShaderFragmentSrc));
	}

	void OnAttach() override
	{
	}

	void OnUpdate(Astro::Timestep ts) override
	{
		if (Astro::Input::IsKeyPressed(AS_KEY_A))
			m_CameraPosition.x -= m_CameraSpeed * ts;
		if (Astro::Input::IsKeyPressed(AS_KEY_D))
			m_CameraPosition.x += m_CameraSpeed * ts;
		if (Astro::Input::IsKeyPressed(AS_KEY_W))
			m_CameraPosition.y += m_CameraSpeed * ts;
		if (Astro::Input::IsKeyPressed(AS_KEY_S))
			m_CameraPosition.y -= m_CameraSpeed * ts;

		if (Astro::Input::IsKeyPressed(AS_KEY_LEFT))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Astro::Input::IsKeyPressed(AS_KEY_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		if (Astro::Input::IsKeyPressed(AS_KEY_R))
			m_ObjRotation += m_ObjRotationSpeed * ts;
		
		Astro::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Astro::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Astro::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));


		glm::vec4 redColor(0.8, 0.2, 0.3, 1.0);
		glm::vec4 blueColor(0.2, 0.3, 0.8, 1.0);

		std::dynamic_pointer_cast<Astro::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Astro::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * 
					glm::rotate(glm::mat4(1.0f), glm::radians(m_ObjRotation), glm::vec3(0, 1, 0)) * scale;

				Astro::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);

			}
		}

		Astro::Renderer::Submit(m_Shader, m_VertexArray);

		Astro::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	void OnEvent(Astro::Event& event) override
	{
		Astro::EventDispatcher dispatcher(event);

		dispatcher.Dispatch<Astro::KeyPressedEvent>(AS_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));
	}

	bool OnKeyPressed(Astro::KeyPressedEvent& event) 
	{
		return false;
	}

private:
	Astro::Ref<Astro::Shader> m_Shader;
	Astro::Ref<Astro::VertexArray> m_VertexArray;

	Astro::Ref<Astro::Shader> m_FlatColorShader;
	Astro::Ref<Astro::VertexArray> m_SquareVA;

	Astro::OrtographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 2.0f;
	
	float m_CameraRotation;
	float m_CameraRotationSpeed = 180.0f;

	float m_ObjRotation;
	float m_ObjRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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