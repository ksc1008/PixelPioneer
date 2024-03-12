#include "tempStorage.h"
#include "chunk.h"

TemporaryStorage TemporaryStorage::instance;

void TemporaryStorage::inititateCube(int size)
{
    if (initiated)
        return;
    cube = new unsigned int[108] {
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

    uvs = new unsigned int[36] {
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
            2,
            3,
            1,
            1,
            0,
            2,

            // Front face
            0,
            2,
            3,
            3,
            1,
            0,

            // Left face
            0,
            2,
            3,
            3,
            1,
            0,

            // Right face
            2,
            3,
            1,
            1,
            0,
            2
    };

    buffer = new unsigned int[size];

}

void TemporaryStorage::initiateFaceTypes()
{
    if (facetypeInitiated)
        return;
    faceTypes =
        new int*** [6];
    for (int i = 0; i < 6; i++) {
        faceTypes[i] = new int** [Chunk::CHUNK_SIZE];
        for (int j = 0; j < Chunk::CHUNK_SIZE; j++) {
            faceTypes[i][j] = new int* [Chunk::CHUNK_SIZE];
            for (int k = 0; k < Chunk::CHUNK_SIZE; k++)
                faceTypes[i][j][k] = new int[Chunk::CHUNK_SIZE];
        }
    }

    ao = new unsigned char*** [6];
    for (int i = 0; i < 6; i++) {
        ao[i] = new unsigned char** [Chunk::CHUNK_SIZE];
        for (int j = 0; j < Chunk::CHUNK_SIZE; j++) {
            ao[i][j] = new unsigned char* [Chunk::CHUNK_SIZE];
            for (int k = 0; k < Chunk::CHUNK_SIZE; k++)
                ao[i][j][k] = new unsigned char[Chunk::CHUNK_SIZE];
        }
    }

    facetypeInitiated = true;
}
