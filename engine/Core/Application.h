#pragma once

#include "Clock.h"
#include "Window.h"
#include "Input.h"
#include <functional>

/*
* This class holds the clock, input, and GLFW window. It also runs the main loop via run()
* It makes sure that the window, input system, and clock are created, running properly, and exit without any errors.
*/

class Application
{
public:
	Application(int width, int height, const char* name);
	Input& getInput();
	Window& getWindow();
	Clock& getClock();
	void run(std::function<void(float dt)> onUpdate);
private:
	Input m_Input;
	Window m_Window;
	Clock m_Clock;
};

