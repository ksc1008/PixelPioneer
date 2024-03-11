#pragma once

#include<string>

struct TexturePackManifest {
public:
	int size;
	std::string name;
	std::string* texturePaths;
	std::string* textureNames;

	~TexturePackManifest() {
		delete texturePaths;
		delete textureNames;
	}
};
TexturePackManifest* testManifest();