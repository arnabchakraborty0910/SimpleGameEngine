#include "PhysicsWorld.h"
#include "engine/scene/Entity.h"

void PhysicsWorld::step(Scene& scene, float frameDt)
{
	accumulator += frameDt;
	if (accumulator > 5 * kFixedDt)
		accumulator = 5 * kFixedDt;

	while (accumulator >= 1.0f / 60) {
		for (Entity& e : scene.entities) {
			e.rigidBody.velocity.y += gravity * kFixedDt;
			e.transform.position += e.rigidBody.velocity * kFixedDt;

			float bottom = e.transform.position.y - e.rigidBody.halfExtents.y;
			if (bottom < floorY) {
				e.transform.position.y = floorY + e.rigidBody.halfExtents.y;
				e.rigidBody.velocity.y = 0;
			}
		}

		for (int i = 0; i + 1 < scene.entities.size(); i++) {
			for (int j = i + 1; j < scene.entities.size(); j++) {
				resolve(scene.entities[i], scene.entities[j]);
			}
		}

		accumulator -= kFixedDt;
	}


}

void PhysicsWorld::resolve(Entity& a, Entity& b)
{
	float dx, dy, dz, ox, oy, oz;


	dx = abs(a.transform.position.x - b.transform.position.x);
	ox = (a.rigidBody.halfExtents.x + b.rigidBody.halfExtents.x) - dx;

	dy = abs(a.transform.position.y - b.transform.position.y);
	oy = (a.rigidBody.halfExtents.y + b.rigidBody.halfExtents.y) - dy;

	dz = abs(a.transform.position.z - b.transform.position.z);
	oz = (a.rigidBody.halfExtents.z + b.rigidBody.halfExtents.z) - dz;

	if (ox <= 0 || oy <= 0 || oz <= 0)
		return;

	if (ox <= oy && ox <= oz) {
		float sign = (a.transform.position.x < b.transform.position.x) ? -1.0f : 1.0f;
		a.transform.position.x += sign * ox * 0.5f;
		b.transform.position.x -= sign * ox * 0.5f;
		a.rigidBody.velocity.x = 0;
		b.rigidBody.velocity.x = 0;
	}
	else if (oy <= ox && oy <= oz) {
		float sign = (a.transform.position.y < b.transform.position.y) ? -1.0f : 1.0f;
		a.transform.position.y += sign * oy * 0.5f;
		b.transform.position.y -= sign * oy * 0.5f;
		a.rigidBody.velocity.y = 0;
		b.rigidBody.velocity.y = 0;
	}
	else {
		float sign = (a.transform.position.z < b.transform.position.z) ? -1.0f : 1.0f;
		a.transform.position.z += sign * oz * 0.5f;
		b.transform.position.z -= sign * oz * 0.5f;
		a.rigidBody.velocity.z = 0;
		b.rigidBody.velocity.z = 0;
	}

}
