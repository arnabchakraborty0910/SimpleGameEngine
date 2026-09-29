#pragma once
class Shader;
class Camera;
class Scene;
class Mesh;
class Light;
#include <glm/glm.hpp>

using namespace glm;
class Renderer
{
public:
	Renderer();
	void beginFrame();
	void setCamera(Shader& shader, Camera& cam, float aspect);
	void draw(Scene& scene, Shader& shader);
	void drawLamp(Shader& shader, Mesh& mesh, Light& light);
};

