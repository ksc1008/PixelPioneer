#include "object.h"
#include "../debug.h"
#include <glm/gtx/euler_angles.hpp>

void GLObject::applyTransform()
{
	// Apply rotation
	auto eulerToQuaternion = glm::eulerAngles(glm::quat(eulerRotation));
	auto X = glm::eulerAngleX(eulerRotation.x);
	auto Y = glm::eulerAngleY(eulerRotation.y);
	auto Z = glm::eulerAngleZ(eulerRotation.z);
	glm::mat4 transform_rotation = X * Y * Z;

	//Apply scale & translation
	matrix_transform = glm::translate(glm::scale(transform_rotation, scale), position);
}


GLObject::GLObject(GLMaterial* material, GLModel* model, glm::vec3 position, glm::vec3 scale, glm::vec3 orientation) {
	this->m_model = model;
	this->position = position;
	this->scale = scale;
	this->eulerRotation = orientation;
	setMaterial(material);

	applyTransform();
}

void GLObject::setPosition(glm::vec3 position){
	this->position = position;
}
void GLObject::translate(glm::vec3 vector){
	this->position += vector;
}
void GLObject::setScale(glm::vec3 scale){
	this->scale = scale;
}
void GLObject::setRotation(glm::vec3 orientation) {
	this->eulerRotation = orientation;
}

void GLObject::setMaterial(GLMaterial* mat) {
	m_material = mat;
}
void GLObject::draw()
{
	m_material->bind();
	ShaderLoader::getInstance()->getDefaultShader()->setModelTransform(this->matrix_transform);

	m_model->bindBuffer();
	m_model->drawMesh();
}

GLMaterial& GLObject::getMaterial()
{
	return *m_material;
}