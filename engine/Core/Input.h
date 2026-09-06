#pragma once

struct GLFWwindow;

class Input
{
public:
	void setWindow(GLFWwindow* window);
	bool isKeyPressed(int key) const;
	void onMouseMove(double xpos, double ypos, float& xoffset, float& yoffset);
private:
	GLFWwindow* handle = nullptr;
	float lastX = 400;
	float lastY = 300;
	bool firstMouse = true;

};

