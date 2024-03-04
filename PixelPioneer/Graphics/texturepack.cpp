#include "texturepack.h"
#include "../Main/debug.h"

Texturepack::Texturepack()
{
	size = 0;
	glTexId = nullptr;
}

void Texturepack::setTexturepack(TexturePackManifest& manifest)
{
	if (!materials.empty()) {
		Debugger::getInstance()->writeLine("material already allocated. need to free exsistant materials.");
		return;
	}

	glGenTextures(manifest.size, glTexId);
	this->size = manifest.size;
	this->name = manifest.name;
	for (int i = 0; i < size; i++) {
		nameToIdMap[manifest.textureNames[i]] = i;
		GLMaterial* mat = new GLMaterial(manifest.texturePaths[i], glTexId[i]);
		materials.push_back(mat);
	}
}

void Texturepack::loadTexturepack()
{
	for (int i = 0; i < size; i++) {
		materials[i]->loadTexture();
	}
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

