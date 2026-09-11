#pragma once

//math classes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
class Light {
public:
	Light(vec3 pos, vec3 col) {
		position = pos;
		color = col;
	}
	vec3 position;
	vec3 color;
	

};