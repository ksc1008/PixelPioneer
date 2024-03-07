#pragma once
#include "../World/chunk.h"

class VoxelModel {
	int size;
	unsigned int VBO, VAO;
	float* buffer;
	bool bufferCreated = false;

public:
	static float* cube;
	static float* uvs;
	static const int VERTEX_SIZE = 5;
	static const int QUAD_VERTICES = 6;

	void startBuild();
	void addQuad(float x, float y, float z, float atlas_x, float atlas_y, int face);
	void endBuild();
	void renderMesh();
};