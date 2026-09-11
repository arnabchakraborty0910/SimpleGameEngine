#pragma once
//math classes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
class RigidBody {
public:
	vec3 velocity = vec3(0.0f);
	vec3 halfExtents = vec3(0.5f);
};