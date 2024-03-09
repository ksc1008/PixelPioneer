#include "texturepack.h"
#include "../debug.h"

Texturepack::Texturepack(TexturePackManifest& manifest)
{
}

void Texturepack::setTexturepack(const TexturePackManifest& manifest)
{
}

void Texturepack::loadTexturepack()
{
}

void Texturepack::bindTexture(const std::string name)
{
}

GLMaterial* Texturepack::getTexture(int id)
{
	return nullptr;
}

GLMaterial* Texturepack::getTexture(std::string name)
{
	return nullptr;
}

int Texturepack::getTextureId(std::string name)
{
	return -1;
}

std::string Texturepack::getName()
{
	return "";
}

void Texturepack::freeMaterials()
{
}

Texturepack::~Texturepack()
{
}

