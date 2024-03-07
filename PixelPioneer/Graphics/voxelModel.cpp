#include "voxelModel.h"

void VoxelModel::startBuild()
{
    if (!cube) {
        cube = new float[108] {
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
    }

    if (!uvs) {
        uvs = new float[72] {
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
    }
    if (!bufferCreated) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        bufferCreated = true;
    }

    buffer = new float[Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * 5 * 36];
	size = 0;
}

void VoxelModel::addQuad(float x, float y, float z, float atlas_x, float atlas_y, int face) {
    int idx = size;
    for (int i = 0; i < QUAD_VERTICES; i++) {
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 0] = cube[18 * face + 3 * i + 0] + x;
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 1] = cube[18 * face + 3 * i + 1] + y;
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 2] = cube[18 * face + 3 * i + 2] + z;
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 3] = uvs[12 * face + 2 * i + 0] + atlas_x;
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 4] = uvs[12 * face + 2 * i + 1] + atlas_y;
    }
    size++;
}

void VoxelModel::endBuild()
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer[0]) * size * VERTEX_SIZE * QUAD_VERTICES, buffer, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    delete buffer;
}

void VoxelModel::renderMesh()
{
}
