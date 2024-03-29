#pragma once
#include <glm/glm.hpp>
enum LightType { POINT, DIRECTIONAL, SPOT };

class Light
{
	LightType m_type;
	glm::vec3 m_pos;
	float m_intensity_ambient;
	float m_intensity_diffuse;

public:
	Light(LightType type, glm::vec3 pos, float intensity);

	void addToShader();
};

