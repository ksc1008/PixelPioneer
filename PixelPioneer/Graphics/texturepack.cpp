#include "texturepack.h"
#include "../debug.h"

Texturepack::Texturepack(TexturePackManifest& manifest)
{
	size = 0;
	glTexId = -1;
	setTexturepack(manifest);
}

void Texturepack::setTexturepack(const TexturePackManifest& manifest)
{
	if (!materials.empty()) {
		Debugger::getInstance()->writeLine("material already allocated. need to free exsistant materials.");
		return;
	}

	glGenTextures(1, &glTexId);
	glBindTexture(GL_TEXTURE_2D_ARRAY, glTexId);
	glTexImage3D(GL_TEXTURE_2D_ARRAY,
		0,                // level
		GL_RGBA,         // Internal format
		16, 16, size, // width,height,depth
		0,
		GL_RGBA,          // format
		GL_UNSIGNED_BYTE, // type
		0);               // pointer to data
	size = manifest.size;
	name = manifest.name;
	for (int i = 0; i < size; i++) {
		nameToIdMap[manifest.textureNames[i]] = i;
		GLMaterial* mat = new GLMaterial(manifest.texturePaths[i], glTexId);
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

