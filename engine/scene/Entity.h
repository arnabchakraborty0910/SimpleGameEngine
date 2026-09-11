#pragma once
#include "Transform.h"
#include "engine/physics/RigidBody.h"
#include <iostream>

class Entity {
public:
	Transform transform;
	RigidBody rigidBody;
	std::string name = "UnknownObject";
	Entity(glm::vec3 pos) : transform{ pos } 
	{

	}
};