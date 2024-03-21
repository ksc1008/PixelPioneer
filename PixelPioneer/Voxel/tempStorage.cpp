#include "tempStorage.h"
#include "chunk.h"

TemporaryStorage TemporaryStorage::instance;

const unsigned int TemporaryStorage::cube[108] = {
    // Bottom face
    0, 0, 0,
        1, 0, 0,
        1, 0, 1,
        1, 0, 1,
        0, 0, 1,
        0, 0, 0,

        // Top face
        0, 1, 0,
        0, 1, 1,
        1, 1, 1,
        1, 1, 1,
        1, 1, 0,
        0, 1, 0,

        // Back face
        0, 0, 0,
        0, 1, 0,
        1, 1, 0,
        1, 1, 0,
        1, 0, 0,
        0, 0, 0,

        // Front face
        0, 0, 1,
        1, 0, 1,
        1, 1, 1,
        1, 1, 1,
        0, 1, 1,
        0, 0, 1,

        // Left face
        0, 0, 0,
        0, 0, 1,
        0, 1, 1,
        0, 1, 1,
        0, 1, 0,
        0, 0, 0,

        // Right face
        1, 0, 0,
        1, 1, 0,
        1, 1, 1,
        1, 1, 1,
        1, 0, 1,
        1, 0, 0
};

const unsigned int TemporaryStorage::uvs[36] = {
    // Bottom face
        2,
        0,
        1,
        1,
        3,
        2,

        // Top face
        2,
        0,
        1,
        1,
        3,
        2,

        // Back face
        0,
        2,
        3,
        3,
        1,
        0,

        // Front face
        0,
        1,
        3,
        3,
        2,
        0,

        // Left face
        0,
        1,
        3,
        3,
        2,
        0,

        // Right face
        1,
        3,
        2,
        2,
        0,
        1
};