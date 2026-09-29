#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <engine/Core/Application.h>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

using namespace glm;
class Camera
{
public:
	vec3 pos;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;

	float yaw;
	float pitch;
	float moveSpeed;
	float mouseSens;
	float zoom;
	Camera(vec3 position = vec3(0.0f), vec3 up = vec3(0.0f,1.0f,0.0f), float camYaw = YAW, float camPitch = PITCH ) : front(vec3(0.0f, 0.0f, -1)), moveSpeed(SPEED), mouseSens(SENSITIVITY), zoom(ZOOM)
	{
		pos = position;
		worldUp = up;
		yaw = camYaw;
		pitch = camPitch;
		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float camYaw, float camPitch) : front(vec3(0, 0, -1)), moveSpeed(SPEED), mouseSens(SENSITIVITY), zoom(ZOOM) 
	{
		pos = vec3(posX, posY, posZ);
		worldUp = vec3(upX, upY, upZ);
		yaw = camYaw;
		pitch = camPitch;
		updateCameraVectors();
	}

	mat4 GetViewMatrix() {
		return lookAt(pos, pos + front, up);
	}

	void ProcessKeyBoard(Input& input, float deltaTime) {
		vec3 dir(0.0f);
		if (input.isKeyPressed(GLFW_KEY_W))
			dir += vec3(front.x, 0.0f, front.z);
		if (input.isKeyPressed(GLFW_KEY_S))
			dir -= vec3(front.x, 0.0f, front.z);
		if (input.isKeyPressed(GLFW_KEY_A))
			dir -= vec3(right.x, 0.0f, right.z);
		if (input.isKeyPressed(GLFW_KEY_D))
			dir += vec3(right.x, 0.0f, right.z);
		if (input.isKeyPressed(GLFW_KEY_SPACE))
			dir += worldUp;
		if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
			dir -= worldUp;

		if(length(dir) > 1e-6f)
		pos += normalize(dir) * moveSpeed * deltaTime;
	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
		xoffset *= mouseSens;
		yoffset *= mouseSens;

		yaw += xoffset;
		pitch += yoffset;

		if (constrainPitch) 
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}
		updateCameraVectors();

	}

	void ProcessMouseScroll(float yoffset) {
		zoom -= yoffset;
		if (zoom < 1.0f)
			zoom = 1.0f;
		if (zoom > 45.0f)
			zoom = 45.0f;
	}


private:
	void updateCameraVectors() {
		vec3 frontCam;
		frontCam.x = cos(radians(yaw)) * cos(radians(pitch));
		frontCam.y = sin(radians(pitch));
		frontCam.z = sin(radians(yaw)) * cos(radians(pitch));

		front = normalize(frontCam);
		right = normalize(cross(front, worldUp));
		up = normalize(cross(right, front));
	}
};

