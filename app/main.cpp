//normal classes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "engine/core/Window.h"
#include "engine/core/Clock.h"
#include "engine/Core/Input.h"
#include "engine/Core/Application.h"
#include "engine/renderer/Mesh.h"
#include "engine/renderer/Texture.h"
#include "engine/renderer/Renderer.h"
#include "engine/scene/Scene.h"
#include "engine/scene/Light.h"
#include "engine/physics/PhysicsWorld.h"


//math classes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//other classes
#include "engine/renderer/Shader.h"
#include "engine/renderer/Camera.h"


using namespace glm;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1280;

//Camera
Camera cam(vec3(0.0f,0.0f,3.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
Application app(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
Scene scene;
PhysicsWorld physics;
Light light(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));



void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float detlaTime);

int main() {
	if (!app.getWindow().getHandle())
		return -1;
	glfwSetInputMode(app.getWindow().getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Mesh
	Mesh cube;

	//shader creation
	Shader shader1("shaders/shader.vert", "shaders/shader.frag");
	Shader lampShader("shaders/light.vert", "shaders/light.frag");

	//textures
	Texture container("textures/container.jpg");
	Texture face("textures/awesomeface.png");
	
	Renderer renderer;

	shader1.use();
	shader1.setInt("texture1", 0);
	shader1.setVec3("lightColor", light.color);
	shader1.setVec3("objectColor", vec3(1.0f, 0.5f, 0.31f));

	lampShader.use();
	lampShader.setVec3("lightColor", light.color);
	lampShader.setVec3("objectColor",vec3( 1.0f, 0.5f, 0.31f));

	//calls this when GLFW detects the size of the window changes
	glfwSetCursorPosCallback(app.getWindow().getHandle(), mouse_callback);
	glfwSetScrollCallback(app.getWindow().getHandle(), scroll_callback);
		
	shader1.use(); // don’t forget to activate the shader first!
	shader1.setInt("texture1", 0);
	shader1.setInt("texture2", 1); // or with shader class
	shader1.setVec3("lightColor", light.color);

	container.bind(0);
	face.bind(1);

	//cubes
	scene.entities.push_back(Entity(vec3(0.0f, 100.0f, 0.0f)));
	scene.entities.push_back(Entity(vec3(0.0f, 150.0f, 0.0f)));
	scene.entities.push_back(Entity(vec3(0.0f, 80.2f, 0.0f)));
	//game loop
	app.run([&](float dt)
	{
		processInput(app.getWindow().getHandle(), dt);

		physics.step(scene, dt);

		renderer.beginFrame();
		renderer.setCamera(shader1, cam, (float)SCR_WIDTH / SCR_HEIGHT);

		//light position
		float t = app.getClock().getTime();
		float radius = 5.0f;
		float height = 2.0f;
		light.position = vec3(
			radius * cos(t),
			height,
			radius * sin(t)
		);
		shader1.use();

		renderer.setCamera(shader1, cam, (float)SCR_WIDTH / SCR_HEIGHT);
		shader1.setVec3("viewPos", cam.pos);
		shader1.setVec3("lightPos", light.position);
		renderer.draw(scene, shader1, cube);

		renderer.setCamera(lampShader, cam, (float)SCR_WIDTH / SCR_HEIGHT);
		renderer.drawLamp(lampShader, cube, light);
		
		//textures
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	});


	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset, yoffset;
	app.getInput().onMouseMove(xpos, ypos, xoffset, yoffset);
	cam.ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window, float deltaTime)
{	
	if (app.getInput().isKeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	
	if (app.getInput().isKeyPressed(GLFW_KEY_W))
		cam.ProcessKeyBoard(FORWARD, deltaTime);
	if (app.getInput().isKeyPressed(GLFW_KEY_S))
		cam.ProcessKeyBoard(BACKWARD, deltaTime);
	if (app.getInput().isKeyPressed(GLFW_KEY_A))
		cam.ProcessKeyBoard(LEFT, deltaTime);
	if (app.getInput().isKeyPressed(GLFW_KEY_D))
		cam.ProcessKeyBoard(RIGHT, deltaTime);


}
