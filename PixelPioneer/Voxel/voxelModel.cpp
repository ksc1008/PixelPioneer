#include "voxelModel.h"
#include "chunk.h"
#include "primitives.h"
#include "../debug.h"

void VoxelModel::startBuild()
{
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        PrimitiveCube::getInstance()->inititateCube(Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * VERTEX_SIZE * QUAD_VERTICES);
    }
	size = 0;
}

void VoxelModel::addQuad(short x, short y, short z, short type, int face) {
    int idx = size;
    short* buffer = PrimitiveCube::getInstance()->buffer;
    const short* cube = PrimitiveCube::getInstance()->cube;
    const short* uvs = PrimitiveCube::getInstance()->uvs;
    for (int i = 0; i < QUAD_VERTICES; i++) {
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 0] = cube[18 * face + 3 * i + 0] + x * 2;
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 1] = cube[18 * face + 3 * i + 1] + y * 2;
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 2] = cube[18 * face + 3 * i + 2] + z * 2;
        buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 3] = uvs[6 * face + i] + 4 * type;

        //Debugger::getInstance()->writeLine("vertex ", i, " = ("
        //    , buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 0],", ",
        //    buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 1],", ",
        //    buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 2],", ",
        //    buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 3], ")");
    }
    size++;
}

void VoxelModel::endBuild()
{
    const short* buffer = PrimitiveCube::getInstance()->buffer;
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(short) * size * VERTEX_SIZE * QUAD_VERTICES, buffer, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribIPointer(0, 4, GL_SHORT, 4 * sizeof(short), (void*)0);
    //glVertexAttribPointer(0, 4, GL_SHORT, GL_FALSE, 4 * sizeof(short), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    //glVertexAttribPointer(1, 1, GL_SHORT, GL_FALSE, 4 * sizeof(short), (void*)(3 * sizeof(short)));
    //glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    Debugger::getInstance()->writeLine("mesh build done. ", "polygon num: ", size * 2);
}

void VoxelModel::renderMesh()
{
    glDrawArrays(GL_TRIANGLES, 0, size * VERTEX_SIZE * QUAD_VERTICES);
}

void VoxelModel::bindVAO() {
    glBindVertexArray(VAO);

}
