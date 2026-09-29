#pragma once
#include <vector>
#include "Entity.h"
#include "Light.h"

class Scene {
public:
	std::vector<Entity>  entities;
	Light light{ glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
};