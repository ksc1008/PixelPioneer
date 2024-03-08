#include "texture.h"

void VoxelTexture::loadTextures(TexturePackManifest manifest)
{
}

void VoxelTexture::createTextureInstance(const int count, const int width, const int height)
{
	glGenTextures(1, &glTexId);
	glBindTexture(GL_TEXTURE_2D_ARRAY, glTexId);
	glTexImage3D(GL_TEXTURE_2D_ARRAY,
		0,                // level
		GL_RGBA,         // Internal format
		width, height, count, // width,height,depth
		0,
		GL_RGBA,          // format
		GL_UNSIGNED_BYTE, // type
		0);               // pointer to data
}

void VoxelTexture::loadTexture(std::string path, const int texId, const int level)
{
}

int VoxelTexture::getFileWidth(std::string path)
{
	return 0;
}
