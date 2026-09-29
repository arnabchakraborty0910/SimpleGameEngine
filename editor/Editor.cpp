#include "Editor.h"
#include "engine/scene/Scene.h"
#include "engine/renderer/Camera.h"
#include "engine/renderer/Mesh.h"
#include "engine/renderer/Texture.h"
#include "imgui.h"

void Editor::draw(Scene& scene, const char* texNames[], int texCount)
{
	ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_Once);
	ImGui::Begin("Editor");
	ImGui::ColorEdit3("Light", &scene.light.color.x);
	ImGui::ColorEdit3("Throw color", &throwColor.x);
	ImGui::Combo("Throw texture", &throwTex, texNames, texCount);
	ImGui::SliderFloat("Throw speed", &throwSpeed, 1.0f, 30.0f);
	ImGui::Text("Press R to throw");
	ImGui::End();
}

void Editor::spawn(Scene& scene, Camera& cam, Mesh& cube, Texture* textures[])
{
	scene.entities.push_back(Entity(cam.pos + cam.front * 2.0f));
	Entity& e = scene.entities.back();
	e.mesh = &cube;
	e.color = throwColor;
	e.texture = textures[throwTex];
	e.rigidBody.mass = 1.0f;
	e.rigidBody.velocity = cam.front * throwSpeed;
	selected = (int)scene.entities.size() - 1;
}