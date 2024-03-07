#pragma once

#include<iostream>
#include <glm/glm.hpp>
#include <glad/glad.h>

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

	~GLMaterial();
};