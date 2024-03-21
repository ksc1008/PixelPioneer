#include "texturepackManifest.h"

TexturePackManifest* testManifest() {
	TexturePackManifest* manifest = new TexturePackManifest;
	manifest->size = 5;
	manifest->name = "test";
	std::string* arrname = new std::string[5]{"stone", "cobblestone","dirt","grass_side","grass_top" };
	std::string* arrpath = new std::string[5]{ 
	"Textures/stone.png","Textures/cobblestone.png",
	"Textures/dirt.png", "Textures/grass_side.png", "Textures/grass_carried.png" };
	manifest->textureNames = arrname;
	manifest->texturePaths = arrpath;

	return manifest;
}