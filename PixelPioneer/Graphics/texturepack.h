#pragma once
#include<iostream>
#include<map>
#include"material.h"
#include<vector>

struct TexturePackManifest {
	int size;
	std::string name;
	std::string* texturePaths;
	std::string* textureNames;
};

class Texturepack {
	std::map<std::string, int> NameToIdMap;
	std::string name;
	std::vector<GLMaterial*> materials;
	int size;
	GLuint* glTexId;

public:
	void SetTexturepack(TexturePackManifest& manifest);
	void LoadTexturepack();

private:

};