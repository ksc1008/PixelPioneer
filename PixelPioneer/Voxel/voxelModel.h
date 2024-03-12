#pragma once

class VoxelModel {
	int size;
	int num_polygons = 0;
	unsigned int VBO, VAO = 0;

	double startTime = 0;
	double endTime = 0;


public:
	static const int VERTEX_SIZE = 3;
	static const int QUAD_VERTICES = 6;

	int getNumberOfPolygons() { return num_polygons; }
	void startBuild();
	void addQuad(int x, int y, int z, int type, int face, int w, int h, unsigned char ao = 0);
	void endBuild();
	void renderMesh();
	void bindVAO();
	~VoxelModel();
};