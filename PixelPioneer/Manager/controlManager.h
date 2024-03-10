#pragma once
#include "../Basics/camera.h"

class ControlManager{
public:
	static ControlManager* getInstance() {
		return &instance;
	}


	void move(float dx, float dy, float dz);

	void look(float dx, float dy);

	void applyCameraToShader();

private:
	float rotationY = 0;

	float speed = 0.1;
	float sensitivity = 0.05;
	static ControlManager instance;
	Camera camera;

	glm::vec3 rotatedForward(float angle, glm::vec3 front);
};