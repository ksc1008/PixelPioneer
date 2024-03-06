#pragma once
#include<cstdio>

struct mesh {
	float* vertices;
	int verticesCount;
public:
	mesh(float* positions, float* uvs, int vCount) {
		this->verticesCount = vCount;
		vertices = new float[5 * vCount];
		for (int i = 0; i < vCount; i++) {
			vertices[5 * i] = positions[3 * i];
			vertices[5 * i + 1] = positions[3 * i + 1];
			vertices[5 * i + 2] = positions[3 * i + 2];
			vertices[5 * i + 3] = uvs[2 * i];
			vertices[5 * i + 4] = uvs[2 * i + 1];
		}
	}
};


class MeshPresets {
public:
	MeshPresets* getInstance() {
		return &instance;
	}

	mesh* cubeWithOneFace() { return m_cubeWithOneFace; }

private:
	MeshPresets();
	static MeshPresets instance;
	mesh* m_cubeWithOneFace;
};

mesh* createCubeWithOneFace();