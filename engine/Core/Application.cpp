#include "Application.h"

Application::Application(int width, int height, const char* name) : m_Window(width, height, name)
{
	if (m_Window.getHandle())
		m_Input.setWindow(m_Window.getHandle());
}

Input& Application::getInput()
{
	return m_Input;
}

Window& Application::getWindow()
{
	return m_Window;
}

Clock& Application::getClock()
{
	return m_Clock;
}

void Application::run(std::function<void(float dt)> onUpdate)
{
	if (!m_Window.getHandle())
		return;

	while (!m_Window.shouldClose()) {
		m_Clock.tick();
		onUpdate(m_Clock.getDT());
		m_Window.swapBuffers();
		m_Window.pollEvents();
	}
}
