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
	float mass = 1.0f;
	float friction = 4.0f;

	float invMass() const { return (mass > 0.0f) ? (1.0f / mass) : 0.0f; }
};