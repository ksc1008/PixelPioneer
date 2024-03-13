#include "voxelModel.h"
#include "chunk.h"
#include "tempStorage.h"
#include "../debug.h"

void VoxelModel::startBuild()
{
    startTime = glfwGetTime();
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        TemporaryStorage::getInstance()->inititateCube(Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * Chunk::CHUNK_SIZE * VERTEX_SIZE * QUAD_VERTICES);
    }
	size = 0;
}

unsigned char getAOBit(unsigned char ao, int vertexIdx) {
    return (ao >> vertexIdx) & 1;
}

void VoxelModel::addQuad(int x, int y, int z, int type, int face, int w, int h, unsigned char ao) {
    int tri_idx[6] = { 0,1,2,2,3,0 };
    int flip_idx[6] = { 1,2,4,4,5,1 };
    int idx = size;
    bool flip = false;
    unsigned int* buffer = TemporaryStorage::getInstance()->buffer;
    const unsigned int* cube = TemporaryStorage::getInstance()->cube;
    const unsigned int* uvs = TemporaryStorage::getInstance()->uvs;

    if ((getAOBit(ao, 0) * getAOBit(ao, 1) * getAOBit(ao, 2))>0 || (getAOBit(ao, 2) * getAOBit(ao, 3) * getAOBit(ao, 0)) > 0) {
        flip = true;
    }

    int xsize = 1; int ysize = 1; int zsize = 1;

    switch (face / 2) {
        case 0: {
            xsize = w;
            zsize = h;
            break;
        }
        case 1: {
            ysize = w;
            xsize = h;
            break;
        }
        case 2: {
            ysize = w;
            zsize = h;
            break;
        }
    }

    if (flip) {
        int t;
        for (int i = 0; i < QUAD_VERTICES; i++) {
            t = flip_idx[i];
            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE] = cube[18 * face + 3 * t + 0] * xsize + x;
            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE] += (cube[18 * face + 3 * t + 1] * ysize + y) * 256;
            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE] += (cube[18 * face + 3 * t + 2] * zsize + z) * 65536;

            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 1] =
                face + 8 * (uvs[6 * face + t] + 4 * (getAOBit(ao, tri_idx[t]) + 2 * type));

            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 2] =
                w + h * 65536;
            //buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 0] = cube[18 * face + 3 * i + 0] + x * 2;
            //buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 1] = cube[18 * face + 3 * i + 1] + y * 2;
            //buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 2] = cube[18 * face + 3 * i + 2] + z * 2;

            /*
                special bit in vertex shader
                SDDDDDDDDDDDDDDDDDDDDDDDDDDDDDCBBAAA (32bit)
                S : sign bit
                D : tex index
                C : ao bit
                B : uv coord bit
                A : normal face bit
            */

        }
    }
    else {
        for (int i = 0; i < QUAD_VERTICES; i++) {
            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE] = cube[18 * face + 3 * i + 0] * xsize + x;
            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE] += (cube[18 * face + 3 * i + 1] * ysize + y) * 256;
            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE] += (cube[18 * face + 3 * i + 2] * zsize + z) * 65536;

            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 1] =
                face + 8 * (uvs[6 * face + i] + 4 * (getAOBit(ao, tri_idx[i]) + 4 * type));

            buffer[idx * VERTEX_SIZE * QUAD_VERTICES + i * VERTEX_SIZE + 2] =
                w + h * 65536;
        }
    }
    size++;
}

void VoxelModel::endBuild()
{
    const unsigned int* buffer = TemporaryStorage::getInstance()->buffer;
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * size * VERTEX_SIZE * QUAD_VERTICES, buffer, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, 3 * sizeof(unsigned int), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    endTime = glfwGetTime();

    num_polygons = size * 2;

    //Debugger::getInstance()->writeLine("mesh build done. ", "polygon num: ", size * 2, ", elapsed time; ",(endTime - startTime)*1000,"ms");
}

void VoxelModel::renderMesh()
{
    glDrawArrays(GL_TRIANGLES, 0, size * VERTEX_SIZE * QUAD_VERTICES);
    //glDrawArrays(GL_LINES, 0, size * VERTEX_SIZE * QUAD_VERTICES);
}

void VoxelModel::bindVAO() {
    glBindVertexArray(VAO);

}

VoxelModel::~VoxelModel()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
