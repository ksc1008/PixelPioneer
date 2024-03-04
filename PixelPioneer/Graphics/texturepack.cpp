#include "texturepack.h"
#include "../Main/debug.h"

void Texturepack::SetTexturepack(TexturePackManifest& manifest)
{
	if (!materials.empty()) {
		Debugger::getInstance()->writeLine("material already allocated. need to free exsistant materials.");
		return;
	}

	glGenTextures(manifest.size, glTexId);
	this->size = manifest.size;
	this->name = manifest.name;
	for (int i = 0; i < size; i++) {
		NameToIdMap[manifest.textureNames[i]] = i;
		GLMaterial* mat = new GLMaterial(manifest.texturePaths[i], glTexId[i]);
		materials.push_back(mat);
	}
}

void Texturepack::LoadTexturepack()
{
	for (int i = 0; i < size; i++) {
		materials[i]->loadTexture();
	}
}
