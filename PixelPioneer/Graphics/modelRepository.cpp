#include "model.h"
#include "../debug.h"
ModelRepository ModelRepository::Instance;

void ModelRepository::addModel(GLModel* model)
{
	nameToIdMap[model->getName()] = top;
	modelMap[top] = model;
	top++;
}

int ModelRepository::findModel(std::string name)
{
	if (nameToIdMap.count(name))
		return nameToIdMap[name];
	return -1;
}

GLModel* ModelRepository::getModel(int id)
{
	if (id == -1) {
		Debugger::getInstance()->writeLine("can't find proper model. returning default model");
		return defaultModel;
	}
	return modelMap[id];
}


void ModelRepository::initialize()
{
	defaultModel = new GLModel(createCubeWithOneFace());
	initialized = true;
}

void ModelRepository::freeModel(int id)
{
	delete modelMap[id];
	modelMap[id] = nullptr;
}
