#include "texturepack.h"

TexturepackRepository TexturepackRepository::Instance;

void TexturepackRepository::addTexturepack(Texturepack* texturepack)
{
	nameToIdMap[texturepack->getName()] = top;
	texturePackMap[top] = texturepack;
	top++;
}

void TexturepackRepository::addTexturepack(TexturePackManifest* manifest)
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
