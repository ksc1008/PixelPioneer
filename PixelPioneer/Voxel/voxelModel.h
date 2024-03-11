#pragma once

class VoxelModel {
	int size;
	unsigned int VBO, VAO = 0;

public:
	static const int VERTEX_SIZE = 4;
	static const int QUAD_VERTICES = 6;

	void startBuild();
	void addQuad(int x, int y, int z, int type, int face);
	void endBuild();
	void renderMesh();
	void bindVAO();
};