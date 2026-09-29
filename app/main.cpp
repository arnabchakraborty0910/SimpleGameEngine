// Demo executable: builds the sample scene and ImGui editor, then Application::run.
// Shader and texture paths are relative to the process working directory
// (Visual Studio: $(ProjectDir)), not this file's folder.
// glad.h must come before glfw3.h.
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/renderer/Shader.h"
#include "engine/renderer/Camera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


/*
* This is the main function. Everything in this program connects back to here
*/

using namespace glm;

//This is the screen width and height for the window
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1280;

//These objects each play a role in the program
Camera cam(vec3(0.0f,0.0f,3.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);  //creates camera 
Application app(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");	//creates the window, input, and clock
Scene scene;	//holds the entities in the scene
PhysicsWorld physics;	//Does all the physics in the world
Editor editor;	//Allows the user to change aspects of the world(lightinng, objects thrown, etc0

bool showEditor = false;
bool tabWasDown = false;

//These are called when certain actions happen on the window
void mouse_callback(GLFWwindow* window, double xpos, double ypos);	//the mouse moves
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);	//scroll wheel 
void processInput(GLFWwindow* window, float detlaTime);		//a keyboard button is pressed
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);	//mouse button is pressed 


int main() {
	//if window isnt created, program returns
	if (!app.getWindow().getHandle())
		return -1;

	//sets the cursor to the middle of the screen and hidden
	glfwSetInputMode(app.getWindow().getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//sets up IMGUI for editor
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(app.getWindow().getHandle(), false); // false: we install GLFW callbacks ourselves
	ImGui_ImplOpenGL3_Init("#version 330");

	//creates meshes for cubes and the ground(plane)
	Mesh cube(Primitive::Cube);
	Mesh plane(Primitive::Plane);

	//creates shaders for the light(lampShader) and for entities(shader1)
	Shader shader1("shaders/shader.vert", "shaders/shader.frag");
	Shader lampShader("shaders/light.vert", "shaders/light.frag");

	//these are the diffrent textures used in the program
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

	//these are lists for the textures that will be put into the editor object
	Texture* throwTextures[] = { &container, &face, &cobble, &clearFloor, &concrete, &detailedFloor, &fancyFloor, &grass, &triangles, &wood };
	const char* throwNames[] = { "container", "face", "cobble", "clearFloor", "concrete", "detailedFloor", "fancyFloor", "grass", "triangles", "wood"};
	
	bool rWasDown = false; //checks if r is pressed

	Renderer renderer; //this draws everything from scene onto the screen 

	//this sets the light shader with the light color and the color of the light source
	lampShader.use();
	lampShader.setVec3("lightColor", scene.light.color);
	lampShader.setVec3("objectColor",vec3( 1.0f, 0.5f, 0.31f));

	//sets the other call backs
	glfwSetCursorPosCallback(app.getWindow().getHandle(), mouse_callback);
	glfwSetScrollCallback(app.getWindow().getHandle(), scroll_callback);
	glfwSetMouseButtonCallback(app.getWindow().getHandle(), mouse_button_callback);

	//this sets the throw speed for the editor
	float throwSpeed = 12.0f;

	//this is the list thing that alocates 256 spots for cubes 
	scene.entities.reserve(256);

	//sets the mesh for all cubes in the scene
	for (Entity& e : scene.entities)
		e.mesh = &cube;

	//this is a lamda function from the app object. Its basically the run funciton thats the main loop of this program
	app.run([&](float dt)
	{
		//checks if certain inputs have happened
		processInput(app.getWindow().getHandle(), dt);

		//this checks if the r key is pressed
		bool rDown = app.getInput().isKeyPressed(GLFW_KEY_R);

		//this spawns a cube when r is pressed
		if (rDown && !rWasDown && !ImGui::GetIO().WantCaptureKeyboard)
			editor.spawn(scene, cam, cube, throwTextures);
		rWasDown = rDown;

		// Tab toggles the editor: free cursor for ImGui, or capture for mouse look.
		if(showEditor)
			glfwSetInputMode(app.getWindow().getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(app.getWindow().getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//this is a single step where physics calculations happen 
		physics.step(scene, dt);

		//this runs everything that happens at the start of a frame
		renderer.beginFrame();

		//this sets camera and the camera matrix
		renderer.setCamera(shader1, cam, (float)SCR_WIDTH / SCR_HEIGHT);

		//binding the floor texture to the ground
		wood.bind(0);
		wood.bind(1);
		
		//creating the floor
		mat4 floorModel(1.0f);
		floorModel = translate(floorModel, vec3(0.0f, -1.0f, 0.0f));
		floorModel = scale(floorModel, vec3(200.0f, 1.0f, 200.0f));
		
		//sets the color of the floor
		shader1.setVec3("objectColor", vec3(1.0f));

		//sets the floor model matrix in the shader and draws the plane
		shader1.setMat4("model", floorModel);
		plane.draw();

		//this makes the light rotate in a circle
		float t = app.getClock().getTime();
		float radius = 6.0f;
		float height = 2.0f;
		scene.light.position = vec3(
			radius * cos(t), 
			height,
			radius * sin(t)
		);

		//binds the container and face into the texture
		container.bind(0);
		face.bind(1);

		//draws all objects in the scene
		renderer.setCamera(shader1, cam, (float)SCR_WIDTH / SCR_HEIGHT);
		shader1.setVec3("viewPos", cam.pos);
		shader1.setVec3("lightColor", scene.light.color * 5.0f);
		shader1.setVec3("lightPos", scene.light.position);
		renderer.draw(scene, shader1);

		//set camera and draw the light
		renderer.setCamera(lampShader, cam, (float)SCR_WIDTH / SCR_HEIGHT);
		renderer.drawLamp(lampShader, cube, scene.light);
		
		glBindVertexArray(0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//sets the editor every loop 
		editor.draw(scene, throwNames, 10);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	});

	//closes the program
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
		app.getInput().onMouseMove(xpos, ypos, xoffset, yoffset); // keep lastX/lastY current so look does not jump
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

	// Edge-triggered Tab: one press toggles the editor, not every frame Tab is held.
	bool tabDown = app.getInput().isKeyPressed(GLFW_KEY_TAB);
	if (tabDown && !tabWasDown)
		showEditor = !showEditor;
	tabWasDown = tabDown;
	
	if (!showEditor) {
		cam.ProcessKeyBoard(app.getInput(), deltaTime);
	}
	


}
