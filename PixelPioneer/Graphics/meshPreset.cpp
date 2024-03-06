#include "mesh.h"

MeshPresets MeshPresets::instance;
MeshPresets::MeshPresets() {
	m_cubeWithOneFace = createCubeWithOneFace();
}

mesh* createCubeWithOneFace() {
	int verticesCount = 36;
	auto pos = new float[108] {
        // Bottom face
        -0.5, 0, -0.5,
            0.5, 0, -0.5,
            0.5, 0, 0.5,
            -0.5, 0, -0.5,
            0.5, 0, 0.5,
            -0.5, 0, 0.5,

            // Top face
            -0.5, 1, -0.5,
            0.5, 1, -0.5,
            0.5, 1, 0.5,
            -0.5, 1, -0.5,
            0.5, 1, 0.5,
            -0.5, 1, 0.5,

            // Front face
            -0.5, 0, 0.5,
            0.5, 0, 0.5,
            0.5, 1, 0.5,
            -0.5, 0, 0.5,
            0.5, 1, 0.5,
            -0.5, 1, 0.5,

            // Back face
            -0.5, 0, -0.5,
            0.5, 0, -0.5,
            0.5, 1, -0.5,
            -0.5, 0, -0.5,
            0.5, 1, -0.5,
            -0.5, 1, -0.5,

            // Left face
            -0.5, 0, -0.5,
            -0.5, 0, 0.5,
            -0.5, 1, 0.5,
            -0.5, 0, -0.5,
            -0.5, 1, 0.5,
            -0.5, 1, -0.5,

            // Right face
            0.5, 0, -0.5,
            0.5, 0, 0.5,
            0.5, 1, 0.5,
            0.5, 0, -0.5,
            0.5, 1, 0.5,
            0.5, 1, -0.5
	};

	auto uvs = new float[72] {
        // Bottom face
        0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,

            // Top face
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,

            // Front face
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,

            // Back face
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,

            // Left face
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,

            // Right face
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            1.0, 1.0,
            0.0, 1.0
	};

	mesh* m = new mesh(pos,uvs,verticesCount);
	delete[] uvs;
	delete[] pos;
	return m;
}