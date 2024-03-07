#define STB_IMAGE_IMPLEMENTATION

#include "material.h"
#include "../include/STB/stb_image.h"
#include "../debug.h"


void GLMaterial::bind() {
	if (!loaded) {
		Debugger::getInstance()->writeLine("Texture not loaded. loading...");
		loadTexture();
	}
	glBindTexture(GL_TEXTURE_2D, m_texId);
}

GLMaterial::GLMaterial(std::string path, GLuint id)
{
	this->path = path;
	loaded = false;
	m_texId = id;
}

void GLMaterial::loadTexture()
{
	Debugger::getInstance()->writeLine("loading texture.");
	if (loaded) {
		Debugger::getInstance()->writeLine("Texture already loaded. discarding existing one.");
		discardTexture();
	}

	data = stbi_load(path.c_str(), &m_width, &m_height, &m_nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	loaded = true;

	glBindTexture(GL_TEXTURE_2D, 0);

}

GLMaterial::~GLMaterial()
{
	Debugger::getInstance()->writeLine("Destroying material.");
}

void GLMaterial::discardTexture()
{
}

void GLMaterial::generateTexture()
{
}
