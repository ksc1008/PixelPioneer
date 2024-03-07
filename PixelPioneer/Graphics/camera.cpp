#include "camera.h"
#include <glm/ext/matrix_clip_space.hpp>

void Camera::applyViewMatrix()
{

}

void Camera::applyProjectionMatrix()
{
	m_projection = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
}

void Camera::setPosition(glm::vec3 position)
{
	m_position = position;
}

void Camera::setRotation(glm::vec3 eulerRotation)
{
	m_eulerRotation = eulerRotation;
}

void Camera::setFrameAspect(float aspect)
{
	m_aspect = aspect;
}

void Camera::setFieldOfView(float fov)
{
	m_fov = fov;
}

void Camera::setViewPlane(float near, float far)
{
	m_nearPlane = near;
	m_farPlane = far;
}

