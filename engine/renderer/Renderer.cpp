#include "Renderer.h"
#include <glad/glad.h>
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "engine/scene/Scene.h"
#include "engine/scene/Light.h"

//math classes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer()
{
	glEnable(GL_DEPTH_TEST);
}

void Renderer::beginFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setCamera(Shader& shader, Camera& cam, float aspect)
{
	mat4 projection = perspective(radians(cam.zoom), aspect, 0.1f, 100.0f);

	shader.use();
	shader.setMat4("view", cam.GetViewMatrix());
	shader.setMat4("projection", projection);

}

void Renderer::draw(Scene& scene, Shader& shader, Mesh& mesh) {
	int i = 0;
	for (Entity& e : scene.entities) {
		mat4 model = mat4(1.0f);
		model = translate(model, e.transform.position);
		shader.setMat4("model", model);
		mesh.draw();
		i++;
	}
}

void Renderer::drawLamp(Shader& shader, Mesh& mesh, Light& light)
{
	shader.setVec3("lightPos", light.position);
	shader.setVec3("lightColor", light.color);

	mat4 identity = mat4(1.0f);
	identity = translate(identity, light.position);
	identity = scale(identity, vec3(0.2f));
	shader.setMat4("model", identity);
	mesh.draw();

}



