#include "camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "../Graphics/shaderLoader.h"

void Camera::applyViewMatrix()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	auto cameraFront = glm::normalize(direction);
	m_view = glm::lookAt(m_position, m_position + cameraFront, cameraUp);
}

void Camera::applyProjectionMatrix()
{
	m_projection = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
}

Camera::Camera()
{
	m_position = glm::vec3(3, 10, 3);
	setFrameAspect(1280, 720);
	setFieldOfView(45.0f);
	setViewPlane(0.1f, 1000);
	applyProjectionMatrix();
	applyViewMatrix();
}

void Camera::rotateY(float rotateDegree)
{
	m_pitch += rotateDegree;
	if (m_pitch > MAX_PITCH)
		m_pitch = MAX_PITCH;
	else if (m_pitch < MIN_PITCH)
		m_pitch = MIN_PITCH;
	applyViewMatrix();
}

void Camera::rotateX(float rotateDegree)
{
	m_yaw = m_yaw + rotateDegree;
	if (m_yaw < 0)
		m_yaw += 360;
	else if (m_yaw > 360)
		m_yaw -= 360;
	applyViewMatrix();
}

void Camera::translate(glm::vec3 position) {
	m_position += position;
	applyViewMatrix();
}

void Camera::setPitch(float pitch)
{
	m_pitch = pitch;
	applyViewMatrix();
}

void Camera::setYaw(float yaw)
{
	m_yaw = yaw;
	applyViewMatrix();
}

void Camera::setPosition(glm::vec3 position) {
	m_position = position;
	applyViewMatrix();
}

void Camera::setFrameAspect(float aspect)
{
	m_aspect = aspect;
}

void Camera::setFieldOfView(float fov)
{
	m_fov = glm::radians(fov);
}

void Camera::setViewPlane(float near, float far)
{
	m_nearPlane = near;
	m_farPlane = far;
}

void Camera::applyToShader()
{
	ShaderLoader::getInstance()->getDefaultShader()->setViewTransform(m_view);
	ShaderLoader::getInstance()->getDefaultShader()->setProjTransform(m_projection);
}

