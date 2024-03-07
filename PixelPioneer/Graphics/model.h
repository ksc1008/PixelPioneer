#pragma once
#include"mesh.h"
#include<map>
#include<vector>
#include<string>
#include <glad/glad.h>

class GLModel {

	std::string name;
	unsigned int VBO, VAO;
	mesh* m_mesh;

public:
	std::string getName() { return name; }
	GLModel(mesh* mesh);
	void bindBuffer();
	void setMesh(mesh* mesh);
	void drawMesh();
};

class ModelRepository {
	int top = 0;
	std::map<std::string, int> nameToIdMap;
	std::map<int, GLModel*> modelMap;
	GLModel* defaultModel;

public:
	static ModelRepository* getInstance() { 
		if (!Instance.initialized)
			Instance.initialize();
		return &Instance; }
	void addModel(GLModel* texturepack);
	int findModel(std::string name);
	GLModel* getModel(int id);

private:
	void initialize();
	bool initialized = false;
	static ModelRepository Instance;
	void freeModel(int id);
};
