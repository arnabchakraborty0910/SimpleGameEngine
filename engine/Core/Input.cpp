#include "Input.h"
#include <GLFW/glfw3.h>

void Input::setWindow(GLFWwindow* window)
{
	handle = window;
}

bool Input::isKeyPressed(int key) const
{
	return glfwGetKey(handle, key) == GLFW_PRESS;
}

void Input::onMouseMove(double xpos, double ypos, float& xoffset, float& yoffset)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	xoffset = xpos - lastX;
	yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
}