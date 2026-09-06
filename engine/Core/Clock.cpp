#include "Clock.h"

#include "Clock.h"
#include <GLFW/glfw3.h>

void Clock::tick()
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

float Clock::getDT() const
{
    return deltaTime;
}

float Clock::getTime() const
{
    return glfwGetTime();
}
