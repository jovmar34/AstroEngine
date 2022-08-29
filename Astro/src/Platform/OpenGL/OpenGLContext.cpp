#include "aspch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Astro {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		: m_windowHandle(windowHandle)
	{
		AS_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AS_CORE_ASSERT(status, "Failed to initialize Glad!");

		AS_CORE_INFO("OpenGL Renderer: \n{0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}