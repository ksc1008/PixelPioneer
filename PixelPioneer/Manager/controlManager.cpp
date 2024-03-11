#include "controlManager.h"

ControlManager ControlManager::instance;

void ControlManager::move(float dx, float dy, float dz, float dt)
{
	camera.translate(glm::vec3(0, dy, 0) * speed * dt);
	camera.translate(rotatedForward(rotationY,glm::vec3(dz,0,dx)) * speed * dt);
}

void ControlManager::look(float dx, float dy)
{
	rotationY += dx * sensitivity;
	if (rotationY < 0)
		rotationY += 360;
	else if (rotationY > 360)
		rotationY -= 360;

	camera.setYaw(rotationY);
	camera.rotateY(dy * sensitivity);
}

void ControlManager::applyCameraToShader()
{
	camera.applyToShader();
}

glm::vec3 ControlManager::rotatedForward(float angle, glm::vec3 front)
{
	float rad = glm::radians(-angle);
	float x = front.x * glm::cos(rad) + glm::sin(rad) * front.z;
	float z = -front.x * glm::sin(rad) + glm::cos(rad) * front.z;
	return glm::vec3(x,0,z);
}
