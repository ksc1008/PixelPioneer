#pragma once
#include "chunk.h"
#include "../Utils/fpsCounter.h"

class ChunkLoader
{
	FpsCounter fpsCounter;
	int m_height, m_width, m_depth;
	Chunk**** m_chunks;
	bool*** loaded;
	bool fast = true;
	bool m_ao_enabled = true;



	void generateChunks(int w, int h, int d);
	void loadChunk(int x, int y, int z);
	void updateChunk(int x, int y, int z);
	void unloadChunk(int x, int y, int z);
	void initiateChunks();


public:
	void updateAdjacentChunkExistanceBitmask(int chunkX, int chunkY, int chunkZ, int face, int i, int j, bool enabled, bool pendUpdate = false);
	unsigned long long getAdjacentExistanceBitmask(int chunkX, int chunkY, int chunkZ, int face, int i, int j);

	void setWorldSize(int w, int h, int d);
	void generateLargeChunk(int x, int y, int z, int n);
	void renderChunks(GLenum renderFace = GL_FRONT, GLenum renderMode = GL_TRIANGLES);
	void updateChunks();
	void loadChunks();
	void switchMeshUpdateMode();
	void setAO(bool enabled);
};


