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
#include "editor/Editor.h"


//math classes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//other classes
#include "engine/renderer/Shader.h"
#include "engine/renderer/Camera.h"

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


using namespace glm;
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1280;

//Camera
Camera cam(vec3(0.0f,0.0f,3.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
Application app(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
Scene scene;
PhysicsWorld physics;
Editor editor;

bool showEditor = false;
bool tabWasDown = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, float detlaTime);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


int main() {
	if (!app.getWindow().getHandle())
		return -1;
	glfwSetInputMode(app.getWindow().getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(app.getWindow().getHandle(), false);
	ImGui_ImplOpenGL3_Init("#version 330");

	
	//Mesh
	Mesh cube(Primitive::Cube);
	Mesh plane(Primitive::Plane);

	//shader creation
	Shader shader1("shaders/shader.vert", "shaders/shader.frag");
	Shader lampShader("shaders/light.vert", "shaders/light.frag");

	//textures
	Texture container("textures/container.jpg");
	Texture face("textures/awesomeface.png");
	Texture cobble("textures/cobbleBlock.jpg");	
	Texture clearFloor("textures/clearFloor.jpg");
	Texture concrete("textures/concrete.jpg");
	Texture detailedFloor("textures/detailedFloor.jpg");
	Texture fancyFloor("textures/fancyFloor.jpg");
	Texture grass("textures/grass.jpg");
	Texture triangles("textures/Triangles.jpg");
	Texture wood("textures/wood.jpg");

	Texture* throwTextures[] = { &container, &face, &cobble, &clearFloor, &concrete, &detailedFloor, &fancyFloor, &grass, &triangles, &wood };
	const char* throwNames[] = { "container", "face", "cobble", "clearFloor", "concrete", "detailedFloor", "fancyFloor", "grass", "triangles", "wood"};
	
	bool rWasDown = false;

	Renderer renderer;

	shader1.use();
	shader1.setInt("texture1", 0);
	shader1.setVec3("objectColor", vec3(1.0f, 0.5f, 0.31f));

	lampShader.use();
	lampShader.setVec3("lightColor", scene.light.color);
	lampShader.setVec3("objectColor",vec3( 1.0f, 0.5f, 0.31f));

	//calls this when GLFW detects the size of the window changes
	glfwSetCursorPosCallback(app.getWindow().getHandle(), mouse_callback);
	glfwSetScrollCallback(app.getWindow().getHandle(), scroll_callback);
	glfwSetMouseButtonCallback(app.getWindow().getHandle(), mouse_button_callback);
		
	shader1.use(); // don’t forget to activate the shader first!
	shader1.setInt("texture1", 0);
	shader1.setInt("texture2", 1); // or with shader class
	shader1.setVec3("lightColor", scene.light.color);


	float throwSpeed = 12.0f;
	scene.entities.reserve(256);


	for (Entity& e : scene.entities)
		e.mesh = &cube;


	//game loop
	app.run([&](float dt)
	{
		processInput(app.getWindow().getHandle(), dt);

		bool rDown = app.getInput().isKeyPressed(GLFW_KEY_R);
		if (rDown && !rWasDown && !ImGui::GetIO().WantCaptureKeyboard)
			editor.spawn(scene, cam, cube, throwTextures);
		rWasDown = rDown;

		if(showEditor)
			glfwSetInputMode(app.getWindow().getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(app.getWindow().getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		physics.step(scene, dt);

		renderer.beginFrame();
		renderer.setCamera(shader1, cam, (float)SCR_WIDTH / SCR_HEIGHT);

		wood.bind(0);
		wood.bind(1);
		
		mat4 floorModel(1.0f);
		floorModel = translate(floorModel, vec3(0.0f, -1.0f, 0.0f));
		floorModel = scale(floorModel, vec3(200.0f, 1.0f, 200.0f));
		
		shader1.setVec3("objectColor", vec3(1.0f));

		shader1.setMat4("model", floorModel);
		plane.draw();

		//light position
		float t = app.getClock().getTime();
		float radius = 6.0f;
		float height = 2.0f;
		scene.light.position = vec3(
			radius * cos(t), 
			height,
			radius * sin(t)
		);

		container.bind(0);
		face.bind(1);

		renderer.setCamera(shader1, cam, (float)SCR_WIDTH / SCR_HEIGHT);
		shader1.setVec3("viewPos", cam.pos);
		shader1.setVec3("lightColor", scene.light.color * 5.0f);
		shader1.setVec3("lightPos", scene.light.position);
		renderer.draw(scene, shader1);

		renderer.setCamera(lampShader, cam, (float)SCR_WIDTH / SCR_HEIGHT);
		renderer.drawLamp(lampShader, cube, scene.light);
		
		//textures
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		editor.draw(scene, throwNames, 10);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	});

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	if (showEditor || ImGui::GetIO().WantCaptureMouse) {
		float xoffset, yoffset;
		app.getInput().onMouseMove(xpos, ypos, xoffset, yoffset); // keep lastX/lastY current
		return;
	}
	
	float xoffset, yoffset;
	app.getInput().onMouseMove(xpos, ypos, xoffset, yoffset);
	cam.ProcessMouseMovement(xoffset, yoffset, true);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	cam.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window, float deltaTime)
{	
	if (app.getInput().isKeyPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	bool tabDown = app.getInput().isKeyPressed(GLFW_KEY_TAB);
	if (tabDown && !tabWasDown)
		showEditor = !showEditor;
	tabWasDown = tabDown;
	
	if (!showEditor) {
		cam.ProcessKeyBoard(app.getInput(), deltaTime);
	}
	


}
