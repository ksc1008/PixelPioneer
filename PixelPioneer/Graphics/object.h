#pragma once
#include "model.h"
#include "texturepack.h"
#include "shaderLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLObject {
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 eulerRotation;


	glm::mat4 matrix_transform;
	GLModel* m_model;
	GLMaterial* m_material;

	void applyTransform();

public:
	GLObject(std::string matName, std::string modelName, glm::vec3 position, glm::vec3 scale, glm::vec3 orientation)
		:GLObject(TexturepackRepository::getInstance()->findTexture(matName),
			ModelRepository::getInstance()->getModel(ModelRepository::getInstance()->findModel(modelName)),
			position, scale, orientation)
	{
	}

	GLObject(std::string matName, std::string modelName, glm::vec3 position)
		:GLObject(matName, modelName, position, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0, 0, 0))
	{
	}

	GLObject(GLMaterial* material, GLModel* model, glm::vec3 position)
		:GLObject(material, model, position, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0, 0, 0))
	{
	}

	GLObject(GLMaterial* material, GLModel* model, glm::vec3 position, glm::vec3 scale, glm::vec3 orientation);

	void setMaterial(GLMaterial* material);
	GLMaterial& getMaterial();

	void setPosition(glm::vec3 position);
	void translate(glm::vec3 vector);
	void setScale(glm::vec3 scale);
	void setRotation(glm::vec3 orientation);
	void draw();
};