#pragma once
#include "block.h"
#include "voxelModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Chunk {
	int m_chunkX, m_chunkY, m_chunkZ;
	VoxelModel* m_model;
public:
	static const int CHUNK_SIZE = 16;
	Chunk(int x, int y, int z);
	~Chunk();
	void update(float dt);
	void render();

	void setBlock(int type, int x, int y, int z);
	void setBlockEnabled(bool enabled, int x, int y, int z);
	void bind();
private: // The blocks data
	Block*** m_pBlocks;
	bool needRefresh = true;

	void createMesh();

	bool checkAdjacent(int x, int y, int z, int face);
	void addFace(int x, int y, int z, int face);
};

