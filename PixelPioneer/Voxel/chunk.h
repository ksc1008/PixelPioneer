#pragma once
#include "block.h"
#include "voxelModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <mutex>

enum RenderMode {FAST, OPTIMAL};
enum RenderState{UPDATING, UPTODATE, ENDUPDATE, NEEDUPDATE, NOTLOADED};
typedef uint_fast64_t bitType;

class Chunk {
	static const short faceOffsets[6][3];
	static const short xPerFace[6][3];
	static const short yPerFace[6][3];
	static const short zPerFace[6][3];
	std::mutex meshStateMutex;
	int m_chunkX, m_chunkY, m_chunkZ;
	VoxelModel* m_model;
	RenderMode m_rendermode = OPTIMAL;
	RenderState m_currentRenderState = NOTLOADED;
	bool m_ao_built = false;
	
public:
	static const int CHUNK_SIZE = 16;
	Chunk(int x, int y, int z);
	~Chunk();
	void update(float dt, bool createAO = true);
	void finishUpdate();
	void render();

	void setBlock(int type, int x, int y, int z, bool pendUpdate = false);
	void setBlockEnabled(bool enabled, int x, int y, int z, bool pendUpdate = false);
	void bind();
	void updateAllMasks();

	void setRenderState(RenderState state);
	RenderState getRenderState();

	int getPolygonNumber();
	void SetRenderMode(RenderMode mode);

	const bitType getExsistanceBitmask(int face, int i,int j) { 
		if (i < 0 || i >= CHUNK_SIZE || j < 0 || j >= CHUNK_SIZE)
			return 0;
		return m_existance_bitmask[face][i][j]; }
	const bitType* getAOBitmask(int face, int depth, int corner) { return m_ao_bitmask[corner][face][depth]; }
	const bitType* getCullingBitmask(int face, int depth) { return m_block_cull_bitmask[face][depth]; }
private: // The blocks data
	Block*** m_pBlocks;

	// [face][direction][depth]
	bitType** m_existance_bitmask[6];
	bitType** m_ao_bitmask[4][6];
	bitType** m_block_cull_bitmask[6];

	unsigned char*** m_horizontal_merge[6];

	bool needRefreshMesh = true;
	bool needRefreshMask = true;

	void createFastMesh(bool createAO);
	void createGreedyMesh(bool createAO);

	void updateCullingMask(int face, int depth, int j);
	void updateAdjacentCullingMask(int x, int y, int z);
	void updateAdjacentAO(int x, int y, int z);
	void updateAOMask(int face, int depth, int i);

	void updateHorizontalMerge(int face, int depth, int i, int j, int type);
	void updateHorizontalMerge(int face, int depth, int i, int j);
	void horizontalMergeUpdateNegativePortion(int face, int depth, int i, int j, int type, unsigned char ao, int x, int y, int z, int value);
	unsigned char horizontalMergeUpdatePositivePortion(int face, int depth, int i, int j, int type, unsigned char ao, int x, int y, int z);
	void updateAdjacentHorizontalMerge(int x, int y, int z, int type);

	unsigned int getAOBitmask(int face, int i, int j, int depth);

	int getX(int face, int depth, int i, int j) {
		return xPerFace[face][0] * depth + xPerFace[face][1] * i + xPerFace[face][2] * j;
	}
	int getY(int face, int depth, int i, int j) {
		return yPerFace[face][0] * depth + yPerFace[face][1] * i + yPerFace[face][2] * j;
	}
	int getZ(int face, int depth, int i, int j) {
		return zPerFace[face][0] * depth + zPerFace[face][1] * i + zPerFace[face][2] * j;
	}
};

