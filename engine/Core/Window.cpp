#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>


Window::Window(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creating window
	m_Window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		m_Window = nullptr;
		glfwTerminate();
		return;
	}
	//current context if things run correctly
	glfwMakeContextCurrent(m_Window);

	//initializing glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		m_Window = nullptr;
		return;
	}

	//size of window on openGL
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
}

GLFWwindow* Window::getHandle()
{
	return m_Window;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window::~Window()
{
	glfwTerminate();
}

int Window::shouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_Window);
}

void Window::pollEvents()
{
	glfwPollEvents();
}
