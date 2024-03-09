#pragma once
#include"../Graphics/texturepack.h"

class VoxelTexture {
	int size;
	int m_width, m_height, m_nrChannels;
	GLuint m_texId = -1;

public:
	VoxelTexture(TexturePackManifest& manifest);
	~VoxelTexture();

	void loadTextures(TexturePackManifest& manifest);

	void bindTextures();

private:
	void createTextureInstance(const int count, const int width, const int height);

	void loadTexture(std::string path, const int level);

};