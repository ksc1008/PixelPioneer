#include "texturepack.h"
#include "../debug.h"

Texturepack::Texturepack(TexturePackManifest* manifest)
{
	size = 0;
	glTexId = nullptr;
	setTexturepack(manifest);
}

void Texturepack::setTexturepack(const TexturePackManifest* manifest)
{
	if (!materials.empty()) {
		Debugger::getInstance()->writeLine("material already allocated. need to free exsistant materials.");
		return;
	}

	glTexId = new GLuint[manifest->size];
	glGenTextures(manifest->size, glTexId);
	size = manifest->size;
	name = manifest->name;
	for (int i = 0; i < size; i++) {
		nameToIdMap[manifest->textureNames[i]] = i;
		GLMaterial* mat = new GLMaterial(manifest->texturePaths[i], glTexId[i]);
		materials.push_back(mat);
	}
}

void Texturepack::loadTexturepack()
{
	for (int i = 0; i < size; i++) {
		materials[i]->loadTexture();
	}
}

void Texturepack::bindTexture(const std::string name)
{
	materials[nameToIdMap[name]]->bind();
}

GLMaterial* Texturepack::getTexture(int id)
{
	if (id == -1)
		return nullptr;
	return materials[id];
}

GLMaterial* Texturepack::getTexture(std::string name)
{
	auto id = getTextureId(name);
	return getTexture(id);
}

int Texturepack::getTextureId(std::string name)
{
	if (nameToIdMap.count(name))
		return nameToIdMap[name];
	return -1;
}

std::string Texturepack::getName()
{
	return name;
}

void Texturepack::freeMaterials()
{
	if (glTexId == nullptr)
		return;

	Debugger::getInstance()->writeLine("removing material instances.");

	for (int i = 0; i < size; i++) {
		delete materials[i];
	}

	glDeleteTextures(size, glTexId);
}

Texturepack::~Texturepack()
{
	Debugger::getInstance()->writeLine("removing texturepack.");
	freeMaterials();
}

