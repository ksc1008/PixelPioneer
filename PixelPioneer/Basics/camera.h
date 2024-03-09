#pragma once
#include <glm/glm.hpp>

class Camera {
	glm::vec3 m_position;
	glm::vec3 m_eulerRotation;

	glm::mat4 m_projection;
	glm::mat4 m_view;

	float m_aspect;
	float m_nearPlane;
	float m_farPlane;
	float m_fov;

	void applyViewMatrix();
	void applyProjectionMatrix();

public:
	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 eulerRotation);

	void setFrameAspect(float aspect);
	void setFrameAspect(int width, int height) {
		setFrameAspect((float)width / (float)height);
	}
	void setFieldOfView(float fov);
	void setViewPlane(float near, float far);
	void setProjection(float fov, float aspect, float near, float far) {
		setFieldOfView(fov);
		setFrameAspect(aspect);
		setViewPlane(near, far);
	}
};