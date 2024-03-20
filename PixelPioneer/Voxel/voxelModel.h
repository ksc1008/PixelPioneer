#pragma once
#include <vector>

class VoxelModel {
	int size;
	int temp_size;
	int num_polygons = 0;
	unsigned int VBO, VAO = 0;

	std::vector<unsigned int> buffer;
	double startTime = 0;
	double endTime = 0;
	unsigned int getAOBit(unsigned int bitmask, int idx);

public:
	static const int VERTEX_SIZE = 3;
	static const int QUAD_VERTICES = 6;

	int getNumberOfPolygons() { return num_polygons; }
	void startBuild();
	void addQuad(int x, int y, int z, int type, int face, int w, int h, unsigned int ao);
	void endBuild();
	void renderMesh();
	void bindVAO();
	~VoxelModel();
};