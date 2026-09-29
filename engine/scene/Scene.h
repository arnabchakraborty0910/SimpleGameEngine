#pragma once

#include <vector>
#include "Entity.h"
#include "Light.h"

/*
* List of entities and a light source. Rederer draws these onto the scene. 
* Physics takes this as well for physics calculations for each object
*/

class Scene {
public:
	std::vector<Entity>  entities;
	Light light{ glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
};