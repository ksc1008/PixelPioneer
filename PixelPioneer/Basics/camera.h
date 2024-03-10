#pragma once
#include <glm/glm.hpp>

class Camera {
	const float MAX_PITCH = 75;
	const float MIN_PITCH = -75;

	glm::vec3 m_position;

	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_yaw = 0;
	float m_pitch = 0;
	float m_roll = 0;

	float m_aspect;
	float m_nearPlane;
	float m_farPlane;
	float m_fov;

	void applyViewMatrix();
	void applyProjectionMatrix();

public:
	Camera();

	void setPosition(glm::vec3 position);
	void rotateY(float rotateDegree);
	void rotateX(float rotateDegree);
	void translate(glm::vec3 position);

	void setPitch(float pitch);
	void setYaw(float yaw);

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
	void applyToShader();
};