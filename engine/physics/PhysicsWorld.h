#pragma once

#include "engine/scene/scene.h"

/*
* This does all the physics calculations on all the entities in the scene except for lighting. 
* 
* Types of calculations include:
*	Gravity
*	collision
*	friction on ground and on other objects
*/

class PhysicsWorld
{
public:
	void step(Scene& scene, float frameDt);
private:
	float accumulator = 0.0f;
	float gravity = -9.81;
	float floorY =-1.0f;
	const float kFixedDt = 1.0f / 60;
	void resolve(Entity& a, Entity& b);

};

