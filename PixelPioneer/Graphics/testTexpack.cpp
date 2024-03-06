#include "texturepack.h"

TexturePackManifest* testManifest() {
	TexturePackManifest* manifest = new TexturePackManifest;
	manifest->size = 3;
	manifest->name = "test";
	std::string* arrname = new std::string[3]{ "grass","cobblestone","stone" };
	std::string* arrpath = new std::string[3]{ "Textures/grass_carried.png","Textures/cobblestone.png","Textures/stone.png" };
	manifest->textureNames = arrname;
	manifest->texturePaths = arrpath;

	return manifest;
}