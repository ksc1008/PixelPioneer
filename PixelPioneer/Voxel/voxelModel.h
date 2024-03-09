#pragma once

class VoxelModel {
	int size;
	unsigned int VBO, VAO = 0;

public:
	static const int VERTEX_SIZE = 4;
	static const int QUAD_VERTICES = 6;

	void startBuild();
	void addQuad(short x, short y, short z, short type, int face);
	void endBuild();
	void renderMesh();
	void bindVAO();
};