#pragma once
#include "Clock.h"
#include "Window.h"
#include "Input.h"
#include <functional>

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

