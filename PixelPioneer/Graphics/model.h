#pragma once
#include"material.h"
#include"mesh.h"

class GLModel {

	unsigned int VBO, VAO, EBO;
	mesh* m_mesh;
	GLMaterial* m_material;

public:
	GLModel(GLMaterial* material, mesh* mesh);
	void bindBuffer();
	void setMaterial(GLMaterial* material);
	void setMesh(mesh* mesh);
	void draw();

};

