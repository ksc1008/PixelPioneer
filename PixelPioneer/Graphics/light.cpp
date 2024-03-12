#include "light.h"
#include "shaderLoader.h"

Light::Light(LightType type, glm::vec3 pos, float intensity)
{
	m_type = type;
	m_pos = pos;
	m_intensity_ambient = 0.4f;
	m_intensity_diffuse = intensity;
}

void Light::addToShader()
{
	ShaderLoader::getInstance()->getDefaultShader()->setFloat("ambientIntensity", m_intensity_ambient);
	ShaderLoader::getInstance()->getDefaultShader()->setFloat("diffuseIntensity", m_intensity_diffuse);
	ShaderLoader::getInstance()->getDefaultShader()->setVec3("lightDirection", m_pos);
	ShaderLoader::getInstance()->getDefaultShader()->setVec3("lightColor", glm::vec3(1.0,1.0,1.0));

}
