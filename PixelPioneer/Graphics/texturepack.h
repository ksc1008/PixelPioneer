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
	std::map<std::string, int> nameToIdMap;
	std::string name;
	std::vector<GLMaterial*> materials;
	int size;
	GLuint* glTexId;

public:
	Texturepack();
	void setTexturepack(TexturePackManifest& manifest);
	void loadTexturepack();

private:
	void freeMaterials();
	~Texturepack();
};

class TexturepackRepository {
	int top = 0;
	std::map<std::string, int> nameToIdMap;
	std::map<int,Texturepack*> texturePackMap;

public:
	static TexturepackRepository* getInstance() { return &Instance; }
	void addTexturepack(Texturepack* texturepack);
	int findTexturepack(std::string name);
	Texturepack* getTexturepack(int id);

private:
	static TexturepackRepository Instance;
	void freeTexturepack(int id);
};