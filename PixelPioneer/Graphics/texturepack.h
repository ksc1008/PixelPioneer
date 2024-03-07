#pragma once
#include<iostream>
#include<map>
#include"material.h"
#include<vector>
#include<string>

struct TexturePackManifest {
public:
	int size;
	std::string name;
	std::string* texturePaths;
	std::string* textureNames;

	~TexturePackManifest(){
		delete texturePaths;
		delete textureNames;
	}
};

class Texturepack {
	std::map<std::string, int> nameToIdMap;
	std::string name;
	std::vector<GLMaterial*> materials;
	int size;
	GLuint* glTexId;

public:
	Texturepack(TexturePackManifest& manifest);

	void setTexturepack(const TexturePackManifest& manifest);
	void loadTexturepack();
	void bindTexture(const std::string name);
	GLMaterial* getTexture(int id);
	GLMaterial* getTexture(std::string name);
	int getTextureId(std::string name);
	std::string getName();
	~Texturepack();

private:
	void freeMaterials();
};

class TexturepackRepository {
	int top = 0;
	std::map<std::string, int> nameToIdMap;
	std::map<int,Texturepack*> texturePackMap;
	static GLMaterial* defaultMat;

public:
	static TexturepackRepository* getInstance() { return &Instance; }
	void addTexturepack(TexturePackManifest& manifest);
	void addTexturepack(Texturepack* texturepack);
	GLMaterial* findTexture(std::string name);
	int findTexturepack(std::string name);
	Texturepack* getTexturepack(int id);

private:
	static TexturepackRepository Instance;
	void freeTexturepack(int id);
};

TexturePackManifest* testManifest();