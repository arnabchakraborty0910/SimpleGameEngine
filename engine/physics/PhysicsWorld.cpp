#include "PhysicsWorld.h"
#include "engine/scene/Entity.h"

void PhysicsWorld::step(Scene& scene, float frameDt)
{
	accumulator += frameDt;
	if (accumulator > 5 * kFixedDt)
		accumulator = 5 * kFixedDt;

	while (accumulator >= 1.0f / 60) {
		for (Entity& e : scene.entities) {
			if (e.rigidBody.invMass() > 0.0f) {
				e.rigidBody.velocity.y += gravity * kFixedDt;
				e.transform.position += e.rigidBody.velocity * kFixedDt;
			}
			float bottom = e.transform.position.y - e.rigidBody.halfExtents.y;
			if (bottom < floorY) {
				e.transform.position.y = floorY + e.rigidBody.halfExtents.y;
				e.rigidBody.velocity.y = 0;

				float damp = 1.0f - e.rigidBody.friction * kFixedDt;
				if (damp < 0.0f) damp = 0.0f;
				e.rigidBody.velocity.x *= damp;
				e.rigidBody.velocity.z *= damp;
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

	//checking if a box is in another box
	dx = abs(a.transform.position.x - b.transform.position.x);
	ox = (a.rigidBody.halfExtents.x + b.rigidBody.halfExtents.x) - dx;

	dy = abs(a.transform.position.y - b.transform.position.y);
	oy = (a.rigidBody.halfExtents.y + b.rigidBody.halfExtents.y) - dy;

	dz = abs(a.transform.position.z - b.transform.position.z);
	oz = (a.rigidBody.halfExtents.z + b.rigidBody.halfExtents.z) - dz;

	if (ox <= 0 || oy <= 0 || oz <= 0)
		return;

	//mass calculation
	float invA = a.rigidBody.invMass();
	float invB = b.rigidBody.invMass();
	float invSum = invA + invB;
	if (invSum <= 0.0f)
		return;

	if (ox <= oy && ox <= oz) {
		float sign = (a.transform.position.x < b.transform.position.x) ? -1.0f : 1.0f;
		a.transform.position.x += sign * ox * (invA / invSum);
		b.transform.position.x -= sign * ox * (invB / invSum);

		float rel = a.rigidBody.velocity.x - b.rigidBody.velocity.x;
		float n = (sign > 0.0f) ? 1.0f : -1.0f;
		float closing = rel * n;
		
		float impulse = 0.0f;
		if (closing < 0.0f) {
			impulse = -closing / invSum;

			a.rigidBody.velocity.x += impulse * invA * n;
			b.rigidBody.velocity.x -= impulse * invB * n;
		}

		//friction
		float mu = 0.4f;
		float maxF = mu * impulse;
		float relY = a.rigidBody.velocity.y - b.rigidBody.velocity.y;
		float jy = -relY / invSum;
		if (jy > maxF) jy = maxF;
		if (jy < -maxF) jy = -maxF;
		a.rigidBody.velocity.y += jy * invA;
		b.rigidBody.velocity.y -= jy * invB;

		float relZ = a.rigidBody.velocity.z - b.rigidBody.velocity.z;
		float jz = -relZ / invSum;
		if (jz > maxF) jz = maxF;
		if (jz < -maxF) jz = -maxF;
		a.rigidBody.velocity.z += jz * invA;
		b.rigidBody.velocity.z -= jz * invB;

	}
	else if (oy <= ox && oy <= oz) {
		float sign = (a.transform.position.y < b.transform.position.y) ? -1.0f : 1.0f;
		a.transform.position.y += sign * oy * (invA / invSum);
		b.transform.position.y -= sign * oy * (invB / invSum);
		float rel = a.rigidBody.velocity.y - b.rigidBody.velocity.y;
		float n = (sign > 0.0f) ? 1.0f : -1.0f;
		float closing = rel * n;

		float impulse = 0.0f;
		if (closing < 0.0f) {
			impulse = -closing / invSum;

			a.rigidBody.velocity.y += impulse * invA * n;
			b.rigidBody.velocity.y -= impulse * invB * n;
		}

		//friction
		float mu = 0.4f;
		float maxF = mu * impulse;
		float relX = a.rigidBody.velocity.x - b.rigidBody.velocity.x;
		float jx = -relX / invSum;
		if (jx > maxF) jx = maxF;
		if (jx < -maxF) jx = -maxF;
		a.rigidBody.velocity.x += jx * invA;
		b.rigidBody.velocity.x -= jx * invB;

		float relZ = a.rigidBody.velocity.z - b.rigidBody.velocity.z;
		float jz = -relZ / invSum;
		if (jz > maxF) jz = maxF;
		if (jz < -maxF) jz = -maxF;
		a.rigidBody.velocity.z += jz * invA;
		b.rigidBody.velocity.z -= jz * invB;
	}
	else {
		float sign = (a.transform.position.z < b.transform.position.z) ? -1.0f : 1.0f;
		a.transform.position.z += sign * oz * (invA / invSum);
		b.transform.position.z -= sign * oz * (invB / invSum);
		
		float rel = a.rigidBody.velocity.z - b.rigidBody.velocity.z;
		float n = (sign > 0.0f) ? 1.0f : -1.0f;
		float closing = rel * n;
		
		float impulse = 0.0f;
		if (closing < 0.0f) {
			impulse = -closing / invSum;

			a.rigidBody.velocity.z += impulse * invA * n;
			b.rigidBody.velocity.z -= impulse * invB * n;
		}

		//friction
		float mu = 0.4f;
		float maxF = mu * impulse;
		float relX = a.rigidBody.velocity.x - b.rigidBody.velocity.x;
		float jx = -relX / invSum;
		if (jx > maxF) jx = maxF;
		if (jx < -maxF) jx = -maxF;
		a.rigidBody.velocity.x += jx * invA;
		b.rigidBody.velocity.x -= jx * invB;

		maxF = mu * impulse;
		float relY = a.rigidBody.velocity.y - b.rigidBody.velocity.y;
		float jy = -relY / invSum;
		if (jy > maxF) jy = maxF;
		if (jy < -maxF) jy = -maxF;
		a.rigidBody.velocity.y += jy * invA;
		b.rigidBody.velocity.y -= jy * invB;
	}

}
