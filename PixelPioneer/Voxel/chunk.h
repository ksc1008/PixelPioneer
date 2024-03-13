#pragma once
#include "block.h"
#include "voxelModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum RenderMode {FAST, OPTIMAL};

class Chunk {
	int m_chunkX, m_chunkY, m_chunkZ;
	VoxelModel* m_model;
	RenderMode m_rendermode = OPTIMAL;
	bool m_ao_built = false;
	
public:
	static const int CHUNK_SIZE = 16;
	Chunk(int x, int y, int z);
	~Chunk();
	void update(float dt, bool createAO = true);
	void render();

	void setBlock(int type, int x, int y, int z);
	void setBlockEnabled(bool enabled, int x, int y, int z);
	void bind();
	int getPolygonNumber();
	void SetRenderMode(RenderMode mode);
private: // The blocks data
	Block*** m_pBlocks;
	bool needRefresh = true;

	void createFastMesh(bool createAO);
	void createGreedyMesh(bool createAO);

	bool checkAdjacent(int x, int y, int z, int face);
	void addFace(int x, int y, int z, int face, unsigned char ao = 0);
	unsigned char checkCorner(int x, int y, int z, int face);

};

