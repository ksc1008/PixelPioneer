#include "voxelModel.h"
#include "chunk.h"
#include "tempStorage.h"
#include "../debug.h"

void VoxelModel::startBuild()
{
    startTime = glfwGetTime();
    buffer.clear();
	temp_size = 0;
}

unsigned int VoxelModel::getAOBit(unsigned int bitmask, int idx) {
    return (bitmask >> idx) & 1;
}

void VoxelModel::addQuad(int x, int y, int z, int type, int face, int w, int h, unsigned int ao) {
    int tri_idx[6] = { 0, 1,2,2,3,0};
    int flip_idx[6] = { 1,2,4,4,5,1 };
    int idx = temp_size;
    bool flip = false;
    const unsigned int* cube = TemporaryStorage::cube;
    const unsigned int* uvs = TemporaryStorage::uvs;

    if (getAOBit(ao,1) + getAOBit(ao, 3) > getAOBit(ao, 0) + getAOBit(ao, 2)) {
        flip = true;
    }

    int xsize = 1; int ysize = 1; int zsize = 1;

    switch (face/2) {
        case 0: {
            zsize = w;
            xsize = h;
            break;
        }
        case 1: {
            xsize = w;
            ysize = h;
            break;
        }
        case 2: {
            ysize = w;
            zsize = h;
            break;
        }
    }

    if (!flip) {
        int t;
        for (int i = 0; i < QUAD_VERTICES; i++) {
            t = flip_idx[i];
            buffer.push_back(cube[18 * face + 3 * t + 0] * xsize + x
                + (cube[18 * face + 3 * t + 1] * ysize + y) * 256
                + (cube[18 * face + 3 * t + 2] * zsize + z) * 65536);

            buffer.push_back(face + 8 * (uvs[6 * face + t] + 4 * (getAOBit(ao, tri_idx[t]) + 2 * type)));

            if (face == 2|| face == 1|| face == 3)
                buffer.push_back(h + w * 65536);
            else 
                buffer.push_back(w + h * 65536);

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
            buffer.push_back(cube[18 * face + 3 * i + 0] * xsize + x
                + (cube[18 * face + 3 * i + 1] * ysize + y) * 256
                + (cube[18 * face + 3 * i + 2] * zsize + z) * 65536);

            buffer.push_back(face + 8 * (uvs[6 * face + i] + 4 * (getAOBit(ao, tri_idx[i]) + 2 * type)));

            if (face == 2 || face == 1 || face == 3)
                buffer.push_back(h + w * 65536);
            else
                buffer.push_back(w + h * 65536);
        }
    }
    temp_size++;
}

void VoxelModel::endBuild()
{
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * buffer.size(), &buffer.front(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribIPointer(0, 3, GL_UNSIGNED_INT, 3 * sizeof(unsigned int), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    endTime = glfwGetTime();

    num_polygons = size * 2;
    size = temp_size;
    buffer.clear();
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
