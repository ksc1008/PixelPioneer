#include "texturepack.h"
#include "../debug.h"



TexturepackRepository TexturepackRepository::Instance;

void TexturepackRepository::addTexturepack(Texturepack* texturepack)
{
	nameToIdMap[texturepack->getName()] = top;
	texturePackMap[top] = texturepack;
	top++;
}

GLMaterial* TexturepackRepository::findTexture(std::string name)
{
	GLMaterial* found = nullptr;
	for (auto pack : texturePackMap) {
		found = pack.second->getTexture(name);
		if (found) {
			return found;
		}
	}

	Debugger::getInstance()->writeLine("can't find proper texturepack. returning null.");
	return nullptr;
}

void TexturepackRepository::addTexturepack(TexturePackManifest& manifest)
{
	Texturepack* texturepack = new Texturepack(manifest);
	texturepack->loadTexturepack();
	nameToIdMap[texturepack->getName()] = top;
	texturePackMap[top] = texturepack;
	top++;
}

int TexturepackRepository::findTexturepack(std::string name)
{
	if (nameToIdMap.count(name))
		return nameToIdMap[name];
	return -1;
}

Texturepack* TexturepackRepository::getTexturepack(int id)
{
	return texturePackMap[id];
}


void TexturepackRepository::freeTexturepack(int id)
{
	delete texturePackMap[id];
	texturePackMap[id] = nullptr;
}
