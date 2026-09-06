#pragma once

struct GLFWwindow;

class Window
{
public: 
	Window(int width, int height, const char* title);
	GLFWwindow* getHandle();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	~Window();
	int shouldClose();
	void swapBuffers();
	void pollEvents();

private:
	GLFWwindow* m_Window;


};

