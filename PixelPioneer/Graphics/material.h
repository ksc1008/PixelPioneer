#pragma once

#include <glm/glm.hpp>
#include "graphics.h"
#include<iostream>

class GLMaterial {
private:
	int m_width, m_height, m_nrChannels;
	GLuint m_texId;
	glm::vec2* uv;
	std::string path;
	unsigned char* data;
	bool loaded;



	void discardTexture();

	void generateTexture();

public:
	GLMaterial(std::string path, GLuint id);

	void bind();
	void loadTexture();
};