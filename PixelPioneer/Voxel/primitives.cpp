#include "primitives.h"

PrimitiveCube PrimitiveCube::instance;

void PrimitiveCube::inititateCube(int size)
{
    if (initiated)
        return;
    cube = new short[108] {
        // Bottom face
        -1, 0, -1,
            1, 0, -1,
            1, 0, 1,
            -1, 0, -1,
            1, 0, 1,
            -1, 0, 1,

            // Top face
            -1, 2, -1,
            -1, 2, 1,
            1, 2, 1,
            -1, 2, -1,
            1, 2, 1,
            1, 2, -1,

            // Front face
            -1, 0, 1,
            1, 0, 1,
            1, 2, 1,
            -1, 0, 1,
            1, 2, 1,
            -1, 2, 1,

            // Back face
            1, 0, -1,
            -1, 0, -1,
            -1, 2, -1,
            1, 0, -1,
            -1, 2, -1,
            1, 2, -1,

            // Left face
            -1, 0, -1,
            -1, 0, 1,
            -1, 2, 1,
            -1, 0, -1,
            -1, 2, 1,
            -1, 2, -1,

            // Right face
            1, 2, -1,
            1, 2, 1,
            1, 0, 1,
            1, 2, -1,
            1, 0, 1,
            1, 0, -1
    };

    uvs = new short[36] {
        // Bottom face
        0,
            2,
            3,
            0,
            3,
            1,

            // Top face
            0,
            2,
            3,
            0,
            3,
            1,

            // Front face
            0,
            2,
            3,
            0,
            3,
            1,

            // Back face
            0,
            2,
            3,
            0,
            3,
            1,

            // Left face
            0,
            2,
            3,
            0,
            3,
            1,

            // Right face
            0,
            2,
            3,
            0,
            3,
            1
    };

    buffer = new short[size];
}
