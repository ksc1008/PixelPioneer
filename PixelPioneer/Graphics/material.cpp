#include "material.h"
#include "../include/STB/stb_image.h"
#include "../Main/debug.h"

void GLMaterial::bind() {
	glBindTexture(GL_TEXTURE_2D, m_texId);
}

GLMaterial::GLMaterial(std::string path, GLuint id)
{
	this->path = path;
	m_texId = 0;
	loaded = false;
	m_texId = id;
}

void GLMaterial::loadTexture()
{
	if (loaded) {
		Debugger::getInstance()->writeLine("Texture already loaded. discarding existing one.");
		discardTexture();
	}

	data = stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, m_texId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	loaded = true;

	glBindTexture(GL_TEXTURE_2D, 0);

}

void GLMaterial::generateTexture()
{
}

void GLMaterial::bind()
{
}
