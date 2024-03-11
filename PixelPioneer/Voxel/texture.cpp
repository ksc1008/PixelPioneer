#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include "../include/STB/stb_image.h"
#include "../debug.h"


VoxelTexture::VoxelTexture(TexturePackManifest& manifest)
{
	loadTextures(manifest);
}

VoxelTexture::~VoxelTexture()
{
	glDeleteTextures(1, &m_texId);
}

void VoxelTexture::loadTextures(TexturePackManifest& manifest)
{
	size = manifest.size;
	for (int i = 0; i < size; i++) {
		loadTexture(manifest.texturePaths[i], i);
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glGenerateMipmap(GL_TEXTURE_3D);
}

void VoxelTexture::bindTextures()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);
}

void VoxelTexture::createTextureInstance(const int count, const int width, const int height)
{
	glGenTextures(1, &m_texId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);
	glTexImage3D(GL_TEXTURE_2D_ARRAY,
		0,                // level
		GL_RGBA,         // Internal format
		width, height, count, // width,height,depth
		0,
		GL_RGBA,          // format
		GL_UNSIGNED_BYTE, // type
		0);               // pointer to data
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_TEXTURE_2D_ARRAY);

}

void VoxelTexture::loadTexture(std::string path, const int level)
{
	Debugger::getInstance()->writeLine("loading texture.");
	
	auto data = stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);

	if (m_texId == -1) {
		createTextureInstance(size, m_width, m_height);
	}
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, level, m_width, m_height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
}
