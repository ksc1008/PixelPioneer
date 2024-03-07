#pragma once
#include "block.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Chunk {
public:
	static const int CHUNK_SIZE = 16;
	Chunk();
	~Chunk();
	void update(float dt);
	void render();
	static const int CHUNK_SIZE = 16;
private: // The blocks data
	Block*** m_pBlocks;
	void createMesh();

	bool xOpaque;
	bool yOpaque;
	bool zOpaque;
};

Chunk::Chunk() { // Create the blocks
	m_pBlocks = new Block **[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pBlocks[i] = new Block * [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_pBlocks[i][j] = new Block[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() { // Delete the blocks
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_pBlocks[i][j];
		}
		delete[] m_pBlocks[i];
	}
	delete[] m_pBlocks;
}