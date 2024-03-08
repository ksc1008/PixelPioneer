#pragma once
#include"../Graphics/texturepack.h"

class VoxelTexture {
	int size;
	GLuint glTexId;

public:
	VoxelTexture(TexturePackManifest manifest);
	~VoxelTexture();

	void loadTextures(TexturePackManifest manifest);

private:
	void createTextureInstance(const int count, const int width, const int height);

	void loadTexture(std::string path, const int texId, const int level);

	int getFileWidth(std::string path);

};