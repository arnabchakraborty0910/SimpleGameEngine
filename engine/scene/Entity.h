#pragma once
#include "Transform.h"
#include "engine/physics/RigidBody.h"
#include <iostream>

class Mesh;
class Texture;

class Entity {
public:
	Transform transform;
	RigidBody rigidBody;
	Mesh* mesh = nullptr;
	Texture* texture = nullptr;
	glm::vec3 color = glm::vec3(1.0f);

	std::string name = "UnknownObject";
	Entity(glm::vec3 pos) : transform{ pos } {}
};